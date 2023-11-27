#pragma vertex

    #version 330 core

    #extension GL_ARB_explicit_uniform_location : enable

    in mediump vec3 a_Position;
    in mediump vec2 a_TexCoord;

    out mediump vec2 v_TexCoord;

    void main() {

        v_TexCoord = a_TexCoord;

        gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_explicit_uniform_location : enable
    #extension GL_ARB_texture_query_levels      : enable

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/rand.glsl"
    #include "/shaders/include/constants.glsl"
    #include "/shaders/include/common_utils.glsl"
    #include "/shaders/include/lighting_utils.glsl"

    in mediump vec2 v_TexCoord;

    /* G-BUFFER */
    layout (location = 0) uniform sampler2D   u_Albedo_gBuffer;
    layout (location = 1) uniform sampler2D u_Emission_gBuffer;
    layout (location = 2) uniform sampler2D u_Material_gBuffer;
    layout (location = 3) uniform sampler2D u_Position_gBuffer;
    layout (location = 4) uniform sampler2D   u_Normal_gBuffer;
    layout (location = 5) uniform sampler2D    u_Depth_gBuffer;

    /* AMBIENT LIGHTING */

    //#define SAMPLER_CUBE

    // Ambient lighting texture.
    #ifdef SAMPLER_CUBE
        layout (location = 98) uniform samplerCube u_Ambient;
    #else
        layout (location = 98) uniform sampler2D u_Ambient;
    #endif

    /* SHADOWS */
    layout (location =  99) uniform sampler2D   u_ShadowMap2D;
    layout (location = 100) uniform samplerCube u_ShadowMap3D;

    uniform mediump mat4 u_LightSpaceMatrix;

    uniform mediump vec2 u_ScreenDimensions;

    uniform mediump float u_AmbientExposure = 1.0; // Brightness of ambient texture.

    uniform mediump vec3 u_CameraPosition;

    const mediump float PCSS_SCENE_SCALE = 0.015625; // Scale of shadow blur (PCSS only).

    uniform mediump float u_ShadowBias       = 0.005; // Shadow bias.
    uniform mediump float u_ShadowNormalBias = 0.1;   // Shadow normal bias.
    uniform mediump float u_NearPlane        = 0.1;   // Light's shadow near plane.

    uniform int u_ShadowSamples = 10; // Number of shadow samples. Please choose a sane value.

    uniform mediump float u_Time;

    /* DIRECT LIGHTING */

    uniform mediump  vec3 u_LightPosition;  // Position of light in world-space.
    uniform mediump  vec3 u_LightDirection; // Direction of the light in world-space.
    uniform mediump float u_LightRange;     // Range of light.
    uniform mediump float u_LightIntensity; // Brightness of light.
    uniform mediump  vec3 u_LightColor;     // Color of light.
    uniform mediump float u_LightSize       =  0.2; // Size of the light (PCSS only).
    uniform mediump float u_LightAngle      = -1.0; // Cos of light's FOV (for spot lights).

    mediump vec3 Irradiance(in mediump vec3 _fresnel, in mediump float _metallic) {
        return mix(vec3(1.0) - _fresnel, vec3(0.0), _metallic);
    }

    mediump float GeomG1(in mediump float _theta, in mediump float _k) {
        return _theta / (_theta * (1.0 - _k) + _k);
    }

    mediump float Geom(in mediump float _cosLi, in mediump float _cosLo, in mediump float _roughness) {

        mediump float r = _roughness + 1.0;
        mediump float k = (r * r) / 8.0;

        return GeomG1(_cosLi, k) * GeomG1(_cosLo, k);
    }

    mediump float Distrib(in mediump float _cosLh, in mediump float _roughness) {

        mediump float alpha = _roughness * _roughness;
        mediump float alphaSq = alpha * alpha;

        mediump float denom = (_cosLh * _cosLh) * (alphaSq - 1.0) + 1.0;
        return alphaSq / (PI * denom * denom);
    }

    mediump vec3 Fresnel(in mediump vec3 _metallic, in mediump float _theta) {
        return _metallic + (vec3(1.0) - _metallic) * pow(1.0 - _theta, 5.0);
    }

    mediump vec3 BRDF(in mediump vec3 _albedo, in mediump vec3 _normal, in mediump vec3 _lightDir, in mediump vec3 _viewDir, in mediump vec3 _halfVec, in mediump float _metallic, in mediump float _roughness) {

        // Calculate angles between surface normal and various light vectors.
        mediump float NdL = max(0.0, dot(_normal, _lightDir));
        mediump float NdH = max(0.0, dot(_normal, _halfVec));
        mediump float NdV = max(0.0, dot(_normal, _viewDir));

        mediump vec3 F0 = mix(vec3(0.04), _albedo, _metallic);

        mediump vec3 fresnel = Fresnel(F0, max(0.0, dot(_halfVec, _viewDir)));

        mediump vec3 diffuse = Irradiance(fresnel, _metallic);

        mediump vec3 specular =
            (fresnel * Distrib(NdH, _roughness) *
            Geom(NdL, NdV, _roughness)) /
            max(KEPSILON, 4.0 * NdL * NdV);

        return (diffuse + specular) * NdL;
    }

    mediump float PCSS_GetOccluderDepth3D(in mediump vec3 _dir, in mediump float _texelSize, in mediump float _bias) {

        mediump float result = 0.0;

        int hits = 0;

        for (int i = 0; i < u_ShadowSamples; i++) {

            mediump vec3 offset = normalize(Random3(_dir + vec3(i + 1), fract(u_Time), 0.1));

            offset *= Random1(_dir.xy + vec2(i + 1), fract(u_Time), 0.4);

            mediump float occluderDepth = Sample1(u_ShadowMap3D, _dir + offset);

            if (occluderDepth < length(_dir) - _bias) {
                result += occluderDepth;

                hits++;
            }
        }

        if (hits == 0) {
            result = -1.0;
        }

        return result / float(max(hits, 1));
    }

    mediump float PCSS_Radius3D(in mediump vec3 _dir, in mediump float _texelSize, in mediump float _bias) {

        mediump float occluderDepth = PCSS_GetOccluderDepth3D(_dir, _texelSize, _bias);

        if (occluderDepth == -1.0) {
            return 1.0;
        }

        mediump float len = length(_dir);

        mediump float penumbraWidth = (len - occluderDepth) / occluderDepth;

        return penumbraWidth * u_LightSize;
    }

    mediump float ShadowCalculationHard3D(in mediump vec3 _dir, in mediump vec3 _offset, in mediump float _bias) {

        // Get depth from light to closest surface.
        mediump float shadowDepth = Sample1(u_ShadowMap3D, _dir + _offset) * u_LightRange;

        // Compare the depth of the light against the distance from the light to the current fragment.
        return length(_dir) - _bias > shadowDepth ? 1.0 : 0.0;
    }

    mediump float ShadowCalculationPCF3D(in mediump vec3 _dir, in mediump float _texelSize, in mediump float _bias, in mediump float _radius) {

        mediump float result = 0.0;

        mediump float hits = 0;

        mediump float PCF_SAMPLES = 8;

        mediump float multiplier = 50.0; // Compensation for 3D.

        mediump float axis = pow(PCF_SAMPLES, 1.0 / 3.0);
        for (mediump float x = -_radius; x < _radius; x += _radius / axis) {
        for (mediump float y = -_radius; y < _radius; y += _radius / axis) {
        for (mediump float z = -_radius; z < _radius; z += _radius / axis) {

            mediump vec3 offset = vec3(x, y, z);

            offset *= _texelSize * multiplier;

            result += ShadowCalculationHard3D(_dir, offset, _bias);

            hits += 1.0;
        }}}

        return result / max(hits, 1.0);
    }

    mediump float ShadowCalculationDisk3D(in mediump vec3 _dir, in mediump float _texelSize, in mediump float _bias, in mediump float _radius) {

        mediump float result = 0.0;

        for (int i = 0; i < u_ShadowSamples; i++) {

            mediump vec3 offset = normalize(Random3(_dir + vec3(i + 1), fract(u_Time), 0.1));

            offset *= _texelSize * _radius * Random1(_dir.xy + vec2(i + 1), fract(u_Time), 0.4);

            result += ShadowCalculationHard3D(_dir, offset, _bias);
        }

        return result / u_ShadowSamples;
    }

    mediump float ShadowCalculationPCSS3D(in mediump vec3 _dir, in mediump  float _texelSize, in mediump float _bias) {

        mediump float result = 0.0;

        mediump float radius = PCSS_Radius3D(_dir, _texelSize, _bias);

        result = ShadowCalculationDisk3D(_dir, _texelSize, _bias, radius);

        return result;
    }

    mediump float TransferShadow3D(in mediump vec3 _normal, in mediump vec3 _lightDir, in mediump vec3 _fragPos, in mediump float _bias, in mediump float _normalBias) {

        mediump float texelSize = 1.0 /
            max(textureSize(u_ShadowMap3D, 0).x, 1);

        mediump vec3 fragToLight = _fragPos - u_LightPosition;

        mediump float perspective_multiplier = 32.0;

        mediump float adjustedBias =
            texelSize * perspective_multiplier * max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

        //return ShadowCalculationHard3D(fragToLight, vec3(0), adjustedBias);
        //return ShadowCalculationPCF3D(fragToLight, texelSize, adjustedBias, 1.0f);
        //return ShadowCalculationDisk3D(fragToLight, texelSize, adjustedBias, 1.0f);
        return ShadowCalculationPCSS3D(fragToLight, PCSS_SCENE_SCALE, adjustedBias);
    }

    mediump float PCSS_GetOccluderDepth2D(in mediump vec3 _fragPos, in mediump float _texelSize, in mediump float _bias) {

        mediump float result = 0.0;

        int hits = 0;

        for (int i = 0; i < u_ShadowSamples; i++) {

            mediump vec2 dir = Random2(_fragPos.xy + vec2(i + 1), fract(u_Time), 0.1);

            dir *= float(i + 1.0) * _texelSize;

            mediump float occluderDepth = Sample1(u_ShadowMap2D, _fragPos.xy + dir);

            if (occluderDepth < _fragPos.z - _bias) {
                result += occluderDepth;

                hits++;
            }
        }

        if (hits == 0) {
            result = -1;
        }

        return result / float(max(hits, 1));
    }

    mediump float PCSS_Radius2D(in mediump vec3 _fragPos, in mediump float _texelSize, in mediump float _bias) {

        mediump float occluderDepth = PCSS_GetOccluderDepth2D(_fragPos, _texelSize, _bias);

        if (occluderDepth == -1.0) {
            return 1.0;
        }

        mediump float penumbraWidth = (_fragPos.z - occluderDepth) / occluderDepth;

        return penumbraWidth * u_LightSize * (u_NearPlane / _fragPos.z);
    }

    mediump float ShadowCalculationHard2D(in mediump vec3 _fragPos, in mediump vec2 _offset, in mediump float _bias) {

        // Get depth from light to closest surface.
        mediump float shadowDepth = Sample1(u_ShadowMap2D, _fragPos.xy + _offset);

        // Compare the depth of the light against the distance from the light to the current fragment (converted to light space).
        return (_fragPos.z <= 1.0) && (_fragPos.z - _bias > shadowDepth) ?
            1.0 : 0.0;
    }

    mediump float ShadowCalculationPCF2D(in mediump vec3 _fragPos, in mediump float _texelSize, in mediump float _bias, in mediump float _radius) {

        mediump float result = 0.0;

        mediump float hits = 0.0;

        mediump float PCF_SAMPLES = 4.0;

        mediump float axis = pow(PCF_SAMPLES, 1.0 / 2.0);
        for (mediump float x = -_radius; x < _radius; x += _radius / axis) {
        for (mediump float y = -_radius; y < _radius; y += _radius / axis) {

            mediump vec2 dir = vec2(x, y);

            dir *= _texelSize;

            result += ShadowCalculationHard2D(_fragPos, dir, _bias);

            hits += 1.0;
        }}

        return result / max(hits, 1.0);
    }

    mediump float ShadowCalculationDisk2D(in mediump vec3 _fragPos, in mediump float _texelSize, in mediump float _bias, in mediump float _radius) {

        mediump float result = 0.0;

        for (int i = 0; i < u_ShadowSamples; i++) {

            mediump vec2 dir = normalize(Random2(_fragPos.xy + vec2(i + 1), fract(u_Time), 0.1));

            dir *= _texelSize * _radius * (float(i + 1) / float(u_ShadowSamples));

            result += ShadowCalculationHard2D(_fragPos, dir, _bias);
        }

        return result / u_ShadowSamples;
    }

    mediump float ShadowCalculationPCSS2D(in mediump vec3 _fragPos, in mediump float _texelSize, in mediump float _bias) {

        mediump float result = 0.0;

        mediump float perspective_multiplier = u_LightAngle == -1.0 ? 1.0 : 32.0;

        mediump float radius = PCSS_Radius2D(_fragPos, _texelSize, _bias) / _texelSize;

        result = ShadowCalculationDisk2D(_fragPos, _texelSize, _bias, radius * perspective_multiplier);

        return result;
    }

    mediump float TransferShadow2D(in mediump vec4 _fragPosLightSpace, in mediump vec3 _normal, in mediump vec3 _lightDir, in mediump float _bias, in mediump float _normalBias) {

        mediump float texelSize = 1.0 /
            max(textureSize(u_ShadowMap2D, 0).x, 1);

        mediump vec3 projCoords =
            ((_fragPosLightSpace.xyz / _fragPosLightSpace.w) * 0.5) + 0.5;

        mediump float perspective_multiplier = u_LightAngle == -1.0 ? 1.0 : 32.0;

        mediump float adjustedBias =
            texelSize * perspective_multiplier * max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

        //return ShadowCalculationHard2D(projCoords, vec2(0), adjustedBias);
        //return ShadowCalculationPCF2D(projCoords, texelSize, adjustedBias, 1.0f);
        //return ShadowCalculationDisk2D(projCoords, texelSize, adjustedBias, 1.0f);
        return ShadowCalculationPCSS2D(projCoords, pow(PCSS_SCENE_SCALE, 2.0) * 2.0, adjustedBias);
    }

    mediump vec3 SampleAmbient(in mediump vec3 _dir, in mediump float _blur) {

        mediump vec3 result;

        mediump float s = textureSize(u_Ambient, 0).x;

        mediump float levels = log2(s);

        int b = int(pow(_blur, levels) * levels);

        #ifdef SAMPLER_CUBE

            // Sample the cubemap the direction directly.
            result = Sample3(u_Ambient, _dir, b);
        #else

            // Sample the texture2d by converting the direction to a uv coordinate.
            // See the example given on: https://en.wikipedia.org/wiki/UV_mapping

            mediump vec2 uv = vec2(
                0.5 + ((atan(_dir.z, _dir.x) / PI) / 2.0),
                0.5 - (asin(_dir.y) / PI)
            );

            // Fix seam at wrap-around point.
            mediump float threshold = 4.0 / textureSize(u_Ambient, b).x;

            mediump float check = step(fract(uv.x - threshold), 1.0 - (threshold * 2.0));

            //result = Sample3(u_Texture, vec2(uv.x * check, uv.y), b);

            // Seemingly no way to do this without a branch.
            // Compiler seems to need an explicit branch on
            // texture access to 'get it'.
            //
            // See for yourself by commenting out the following code
            // and uncommenting the previous code.

            result = check > 0.0 ?
                Sample3(u_Ambient, uv, b) :
                Sample3(u_Ambient, vec2(0.0, uv.y), b);

        #endif

        return result * u_AmbientExposure;
    }

    void main() {

        mediump vec3 albedo   = Sample3(  u_Albedo_gBuffer, v_TexCoord);
        mediump vec3 emission = Sample3(u_Emission_gBuffer, v_TexCoord);
        mediump vec4 material = Sample4(u_Material_gBuffer, v_TexCoord);
        mediump vec3 position = Sample3(u_Position_gBuffer, v_TexCoord);
        mediump vec4 normDisp = Sample4(  u_Normal_gBuffer, v_TexCoord);
        mediump float   depth = Sample1(   u_Depth_gBuffer, v_TexCoord);

        mediump float roughness = material.x;
        mediump float metallic  = material.y;
        mediump float ao        = material.z;
        mediump float ps        = material.w;

        mediump vec3 normal = normDisp.xyz;

        mediump vec3  viewDir = normalize(u_CameraPosition - position);
        mediump vec3 lightDir = normalize( u_LightPosition - position);
        mediump vec3  halfVec = normalize(lightDir + viewDir);

        /* DIRECT */

        mediump vec3 directLighting;
        {
            mediump float attenuation = Attenuation(u_LightPosition, position, u_LightRange);

            mediump vec4 position_lightSpace = u_LightSpaceMatrix * vec4(position, 1.0);

            mediump float visibility = clamp(
                (
                    #define LIGHT_TYPE 2

                    /* DIRECTIONAL */
                    #if LIGHT_TYPE == 0
                        (dot(u_LightDirection, u_LightDirection) > u_LightAngle ? 1.0 : 0.0) *
                        1.0 - max(
                            TransferShadow2D(position_lightSpace, normal, u_LightDirection, u_ShadowBias, u_ShadowNormalBias),
                            ps
                        )
                    /* SPOT */
                    #elif LIGHT_TYPE == 1
                        (dot(u_LightDirection, lightDir) > u_LightAngle ? 1.0 : 0.0) *
                        1.0 - max(
                            TransferShadow2D(position_lightSpace, normal, lightDir, u_ShadowBias, u_ShadowNormalBias),
                            ps
                        )
                    /* POINT */
                    #elif LIGHT_TYPE == 2
                        (dot(u_LightDirection, lightDir) > u_LightAngle ? 1.0 : 0.0) *
                        1.0 - max(
                            TransferShadow3D(normal, lightDir, position, u_ShadowBias, u_ShadowNormalBias),
                            ps
                        )
                    #endif
                ),
                0.0,
                1.0
            );

            mediump vec3 lighting = BRDF(
                albedo.rgb,
                normal,
                lightDir,
                viewDir,
                halfVec,
                metallic,
                roughness
            ) * u_LightIntensity * attenuation;

            directLighting += (visibility * lighting) * u_LightColor;
        }

        directLighting *= albedo.rgb;

        /* INDIRECT */

        mediump vec3 indirectLighting;
        {
            // Figure out the direction of the ambient light
            mediump vec3 ambientDir = reflect(-viewDir, normal);

            // Sample at max mip level for the diffuse.
            // TODO: Actual diffuse irradiance.
            mediump vec3 diffuse = SampleAmbient(normal, 0.95);

            // Sample at variable mip level (determined by roughness) for specular.
            mediump vec3 specular = SampleAmbient(ambientDir, roughness);

            mediump vec3 F0 = mix(vec3(0.04), vec3(1.0), metallic);

            mediump vec3 fresnel = Fresnel(F0, max(0.0, dot(normal, ambientDir)));

            indirectLighting = (((diffuse * (1.0 - metallic)) * albedo.rgb) + (fresnel * specular));
        }

        gl_FragColor = vec4(((directLighting + indirectLighting) * ao) + emission, 1.0);
    }