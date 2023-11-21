#pragma vertex

    #version 330 core

    #extension GL_ARB_explicit_uniform_location: enable

    in vec3 a_Position;
    in vec3 a_Normal;
    in vec2 a_TexCoord;
    in vec3 a_Tangent;
    in vec3 a_Bitangent;

    out vec3 v_Position;
    out vec2 v_TexCoord;
    out vec3 v_Normal;
    out mat3 v_TBN;

    out vec4 v_Position_LightSpace;

    /* PARAMETERS */
    uniform mat4 u_Projection;
    uniform mat4 u_Model;
    uniform mat4 u_View;

    uniform mat4 u_LightSpaceMatrix;

    void main() {

        // Perform perspective projection on model vertex:
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

        // Position in model space:
        v_Position = vec3(u_Model * vec4(a_Position, 1.0));

        // Texture coordinates:
        v_TexCoord = a_TexCoord;

        // Normal:
        v_Normal = transpose(inverse(mat3(u_Model))) * a_Normal;

        // Compute TBN matrix:
        v_TBN = mat3(
            normalize(vec3(u_Model * vec4(a_Tangent,   0))),
            normalize(vec3(u_Model * vec4(a_Bitangent, 0))),
            normalize(vec3(u_Model * vec4(a_Normal,    0)))
        );

        // Position in light space (for shadow calculations):
        v_Position_LightSpace = u_LightSpaceMatrix * vec4(v_Position, 1.0);
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

    in vec2 v_TexCoord;
    in vec3 v_Position;
    in vec3 v_Normal;

    in mat3 v_TBN;

    in vec4 v_Position_LightSpace;

    uniform vec3 u_CameraPosition; // Camera position. Mainly used for lighting calculations.

    /* PBR */
    layout (location = 0) uniform sampler2D u_Albedo;
    layout (location = 1) uniform sampler2D u_Roughness;
    layout (location = 2) uniform sampler2D u_Metallic;
    layout (location = 3) uniform sampler2D u_Normals;
    layout (location = 4) uniform sampler2D u_Displacement;
    layout (location = 5) uniform sampler2D u_Detail;
    layout (location = 6) uniform sampler2D u_AO;
    layout (location = 7) uniform sampler2D u_Emission;

    /* AMBIENT LIGHTING */

    //#define SAMPLER_CUBE

    // Ambient lighting texture.
    #ifdef SAMPLER_CUBE
        layout (location = 98) uniform samplerCube u_Ambient;
    #else
        layout (location = 98) uniform sampler2D u_Ambient;
    #endif

    uniform float u_AmbientExposure = 1.0; // Brightness of ambient texture.

    /* SHADOWS */
    layout (location =  99) uniform sampler2D   u_ShadowMap2D;
    layout (location = 100) uniform samplerCube u_ShadowMap3D;

    const float PCSS_SCENE_SCALE = 0.015625; // Scale of shadow blur (PCSS only).

    uniform float u_ShadowBias       = 0.005; // Shadow bias.
    uniform float u_ShadowNormalBias = 0.1;   // Shadow normal bias.
    uniform float u_NearPlane        = 0.1;   // Light's shadow near plane.

    uniform int u_ShadowSamples = 10; // Number of shadow samples. Please choose a sane value.

    uniform float     u_Metallic_Amount = 0.0; // How metallic the surface is.
    uniform float    u_Roughness_Amount = 0.0; // How rough the surface is.
    uniform float     u_Emission_Amount = 1.0; // How emissive the surface is.
    uniform float       u_Normal_Amount = 1.0; // Contribution of normal map.
    uniform float u_Displacement_Amount = 0.0; // Strength of displacement.
    uniform float           u_AO_Amount = 1.0; // Strength of AO.

    uniform float u_Time;

    /* TEXTURE PARAMETERS */
    // Texture tiling scale and translate (ST) (U, V, U, V).
    uniform vec4 u_ST = vec4(1.0, 1.0, 0.0, 0.0);

    /* DIRECT LIGHTING */

    uniform  vec3 u_LightPosition;  // Position of light in world-space.
    uniform  vec3 u_LightDirection; // Direction of the light in world-space.
    uniform float u_LightRange;     // Range of light.
    uniform float u_LightIntensity; // Brightness of light.
    uniform  vec3 u_LightColor;     // Color of light.
    uniform float u_LightSize       =  0.2; // Size of the light (PCSS only).
    uniform float u_LightAngle      = -1.0; // Cos of light's FOV (for spot lights).

    vec3 Irradiance(vec3 _fresnel, float _metallic) {
        return mix(vec3(1.0) - _fresnel, vec3(0.0), _metallic);
    }

    float GeomG1(float _theta, float _k) {
        return _theta / (_theta * (1.0 - _k) + _k);
    }

    float Geom(float _cosLi, float _cosLo, float _roughness) {

        float r = _roughness + 1.0;
        float k = (r * r) / 8.0;

        return GeomG1(_cosLi, k) * GeomG1(_cosLo, k);
    }

    float Distrib(float _cosLh, float _roughness) {

        float alpha = _roughness * _roughness;
        float alphaSq = alpha * alpha;

        float denom = (_cosLh * _cosLh) * (alphaSq - 1.0) + 1.0;
        return alphaSq / (PI * denom * denom);
    }

    vec3 Fresnel(vec3 _metallic, float _theta) {
        return _metallic + (vec3(1.0) - _metallic) * pow(1.0 - _theta, 5.0);
    }

    vec3 BRDF(vec3 _albedo, vec3 _normal, vec3 _lightDir, vec3 _viewDir, vec3 _halfVec, float _metallic, float _roughness) {

        // Calculate angles between surface normal and various light vectors.
        float NdL = max(0.0, dot(_normal, _lightDir));
        float NdH = max(0.0, dot(_normal, _halfVec));
        float NdV = max(0.0, dot(_normal, _viewDir));

        vec3 F0 = mix(vec3(0.04), _albedo, _metallic);

        vec3 fresnel = Fresnel(F0, max(0.0, dot(_halfVec, _viewDir)));

        vec3 diffuse = Irradiance(fresnel, _metallic);

        vec3 specular =
            (fresnel * Distrib(NdH, _roughness) *
            Geom(NdL, NdV, _roughness)) /
            max(KEPSILON, 4.0 * NdL * NdV);

        return (diffuse + specular) * NdL;
    }

    float PCSS_GetOccluderDepth3D(vec3 _dir, float _texelSize, float _bias) {

        float result = 0.0;

        int hits = 0;

        for (int i = 0; i < u_ShadowSamples; i++) {

            vec3 offset = normalize(Random3(_dir + vec3(i + 1), fract(u_Time), 0.1));

            offset *= Random1(_dir.xy + vec2(i + 1), fract(u_Time), 0.4);

            float occluderDepth = Sample1(u_ShadowMap3D, _dir + offset);

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

    float PCSS_Radius3D(vec3 _dir, float _texelSize, float _bias) {

        float occluderDepth = PCSS_GetOccluderDepth3D(_dir, _texelSize, _bias);

        if (occluderDepth == -1.0) {
            return 1.0;
        }

        float len = length(_dir);

        float penumbraWidth = (len - occluderDepth) / occluderDepth;

        return penumbraWidth * u_LightSize;
    }

    float ShadowCalculationHard3D(vec3 _dir, vec3 _offset, float _bias) {

        // Get depth from light to closest surface.
        float shadowDepth = Sample1(u_ShadowMap3D, _dir + _offset) * u_LightRange;

        // Compare the depth of the light against the distance from the light to the current fragment.
        return length(_dir) - _bias > shadowDepth ? 1.0 : 0.0;
    }

    float ShadowCalculationPCF3D(vec3 _dir, float _texelSize, float _bias, float _radius) {

        float result = 0.0;

        float hits = 0;

        float PCF_SAMPLES = 8;

        float multiplier = 50.0; // Compensation for 3D.

        float axis = pow(PCF_SAMPLES, 1.0 / 3.0);
        for (float x = -_radius; x < _radius; x += _radius / axis) {
        for (float y = -_radius; y < _radius; y += _radius / axis) {
        for (float z = -_radius; z < _radius; z += _radius / axis) {

            vec3 offset = vec3(x, y, z);

            offset *= _texelSize * multiplier;

            result += ShadowCalculationHard3D(_dir, offset, _bias);

            hits += 1.0;
        }}}

        return result / max(hits, 1);
    }

    float ShadowCalculationDisk3D(vec3 _dir, float _texelSize, float _bias, float _radius) {

        float result = 0.0;

        for (int i = 0; i < u_ShadowSamples; i++) {

            vec3 offset = normalize(Random3(_dir + vec3(i + 1), fract(u_Time), 0.1));

            offset *= _texelSize * _radius * Random1(_dir.xy + vec2(i + 1), fract(u_Time), 0.4);

            result += ShadowCalculationHard3D(_dir, offset, _bias);
        }

        return result / u_ShadowSamples;
    }

    float ShadowCalculationPCSS3D(vec3 _dir, float _texelSize, float _bias) {

        float result = 0.0;

        float radius = PCSS_Radius3D(_dir, _texelSize, _bias);

        result = ShadowCalculationDisk3D(_dir, _texelSize, _bias, radius);

        return result;
    }

    float TransferShadow3D(vec3 _normal, vec3 _lightDir, vec3 _fragPos, float _bias, float _normalBias) {

        float texelSize = 1.0 /
            max(textureSize(u_ShadowMap3D, 0).x, 1);

        vec3 fragToLight = _fragPos - u_LightPosition;

        float perspective_multiplier = 32.0;

        float adjustedBias =
            texelSize * perspective_multiplier * max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

        //return ShadowCalculationHard3D(fragToLight, vec3(0), adjustedBias);
        //return ShadowCalculationPCF3D(fragToLight, texelSize, adjustedBias, 1.0f);
        //return ShadowCalculationDisk3D(fragToLight, texelSize, adjustedBias, 1.0f);
        return ShadowCalculationPCSS3D(fragToLight, PCSS_SCENE_SCALE, adjustedBias);
    }

    float PCSS_GetOccluderDepth2D(vec3 _fragPos, float _texelSize, float _bias) {

        float result = 0.0;

        int hits = 0;

        for (int i = 0; i < u_ShadowSamples; i++) {

            vec2 dir = Random2(_fragPos.xy + vec2(i + 1), fract(u_Time), 0.1);

            dir *= float(i + 1.0) * _texelSize;

            float occluderDepth = Sample1(u_ShadowMap2D, _fragPos.xy + dir);

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

    float PCSS_Radius2D(vec3 _fragPos, float _texelSize, float _bias) {

        float occluderDepth = PCSS_GetOccluderDepth2D(_fragPos, _texelSize, _bias);

        if (occluderDepth == -1.0) {
            return 1.0;
        }

        float penumbraWidth = (_fragPos.z - occluderDepth) / occluderDepth;

        return penumbraWidth * u_LightSize * (u_NearPlane / _fragPos.z);
    }

    float ShadowCalculationHard2D(vec3 _fragPos, vec2 _offset, float _bias) {

        // Get depth from light to closest surface.
        float shadowDepth = Sample1(u_ShadowMap2D, _fragPos.xy + _offset);

        // Compare the depth of the light against the distance from the light to the current fragment (converted to light space).
        return (_fragPos.z <= 1.0) && (_fragPos.z - _bias > shadowDepth) ?
            1.0 : 0.0;
    }

    float ShadowCalculationPCF2D(vec3 _fragPos, float _texelSize, float _bias, float _radius) {

        float result = 0.0;

        float hits = 0.0;

        float PCF_SAMPLES = 4.0;

        float axis = pow(PCF_SAMPLES, 1.0 / 2.0);
        for (float x = -_radius; x < _radius; x += _radius / axis) {
        for (float y = -_radius; y < _radius; y += _radius / axis) {

            vec2 dir = vec2(x, y);

            dir *= _texelSize;

            result += ShadowCalculationHard2D(_fragPos, dir, _bias);

            hits += 1.0;
        }}

        return result / max(hits, 1.0);
    }

    float ShadowCalculationDisk2D(vec3 _fragPos, float _texelSize, float _bias, float _radius) {

        float result = 0.0;

        for (int i = 0; i < u_ShadowSamples; i++) {

            vec2 dir = normalize(Random2(_fragPos.xy + vec2(i + 1), fract(u_Time), 0.1));

            dir *= _texelSize * _radius * (float(i + 1) / float(u_ShadowSamples));

            result += ShadowCalculationHard2D(_fragPos, dir, _bias);
        }

        return result / u_ShadowSamples;
    }

    float ShadowCalculationPCSS2D(vec3 _fragPos, float _texelSize, float _bias) {

        float result = 0.0;

        float perspective_multiplier = u_LightAngle == -1.0 ? 1.0 : 32.0;

        float radius = PCSS_Radius2D(_fragPos, _texelSize, _bias) / _texelSize;

        result = ShadowCalculationDisk2D(_fragPos, _texelSize, _bias, radius * perspective_multiplier);

        return result;
    }

    float TransferShadow2D(vec4 _fragPosLightSpace, vec3 _normal, vec3 _lightDir, float _bias, float _normalBias) {

        float texelSize = 1.0 /
            max(textureSize(u_ShadowMap2D, 0).x, 1);

        vec3 projCoords =
            ((_fragPosLightSpace.xyz / _fragPosLightSpace.w) * 0.5) + 0.5;

        float perspective_multiplier = u_LightAngle == -1.0 ? 1.0 : 32.0;

        float adjustedBias =
            texelSize * perspective_multiplier * max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

        //return ShadowCalculationHard2D(projCoords, vec2(0), adjustedBias);
        //return ShadowCalculationPCF2D(projCoords, texelSize, adjustedBias, 1.0f);
        //return ShadowCalculationDisk2D(projCoords, texelSize, adjustedBias, 1.0f);
        return ShadowCalculationPCSS2D(projCoords, pow(PCSS_SCENE_SCALE, 2.0) * 2.0, adjustedBias);
    }

    vec3 SampleAmbient(in vec3 _dir, float _blur) {

        vec3 result;

        float s = textureSize(u_Ambient, 0).x;

        float levels = log2(s);

        int b = int(pow(_blur, levels) * levels);

        #ifdef SAMPLER_CUBE

            // Sample the cubemap the direction directly.
            result = Sample3(u_Ambient, _dir, b);
        #else

            // Sample the texture2d by converting the direction to a uv coordinate.
            // See the example given on: https://en.wikipedia.org/wiki/UV_mapping

            vec2 uv = vec2(
                0.5 + ((atan(_dir.z, _dir.x) / PI) / 2.0),
                0.5 - (asin(_dir.y) / PI)
            );

            // Fix seam at wrap-around point.
            float threshold = 4.0 / textureSize(u_Ambient, b).x;

            float check = step(fract(uv.x - threshold), 1.0 - (threshold * 2.0));

            //result = Sample3(u_Texture, vec2(uv.x * check, uv.y), b);

            // Seemingly no way to do this without a branch.
            // Compiler seems to need an explicit branch on
            // texture access to 'get it'.
            //
            // See for yourself by commenting out the following code
            // and uncommenting the previous code.

            result = check > 0 ?
                Sample3(u_Ambient, uv, b) :
                Sample3(u_Ambient, vec2(0.0, uv.y), b);

        #endif

        return result * u_AmbientExposure;
    }

    float ParallaxShadowsHard(in sampler2D _displacement, in vec3 _lightDir, in vec2 _uv, in vec4 _st, in float _scale) {

        float minLayers = 0;
        float maxLayers = 32;
        float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), _lightDir)));

        vec2 currentTexCoords = _uv;
        float currentDepthMapValue = Sample1(_displacement, currentTexCoords, _st);
        float currentLayerDepth = currentDepthMapValue;

        float layerDepth = 1.0 / numLayers;
        vec2 P = _lightDir.xy / _lightDir.z * _scale;
        vec2 deltaTexCoords = P / numLayers;

        while (currentLayerDepth <= currentDepthMapValue && currentLayerDepth > 0.0) {

            currentTexCoords += deltaTexCoords;
            currentDepthMapValue = Sample1(_displacement, currentTexCoords, _st);
            currentLayerDepth -= layerDepth;
        }

        float r = currentLayerDepth > currentDepthMapValue ? 1.0 : 0.0;
        return r;
    }

    void main() {

        vec3 viewDir_Tangent = normalize(
            (transpose(v_TBN) * normalize(u_CameraPosition - v_Position))
        );

        vec2 uv = ParallaxMapping(
            u_Displacement,
            viewDir_Tangent,
            v_TexCoord,
            u_ST,
            u_Displacement_Amount
        );

        vec3 fragPos = v_Position - (v_Normal * Sample1(u_Displacement, uv, u_ST));

        vec3  viewDir = normalize(u_CameraPosition - fragPos);
        vec3 lightDir = normalize( u_LightPosition - fragPos);
        vec3  halfVec = normalize(lightDir + viewDir);

        vec4     albedo = Sample4(u_Albedo, uv, u_ST);
        float roughness = Sample1(u_Roughness, uv, u_ST) * u_Roughness_Amount;
        float  metallic = Sample1(u_Metallic, uv, u_ST) *  u_Metallic_Amount;

        vec3 normal = normalize((Sample3(u_Normals, uv, u_ST) * 2.0) - 1.0);
        normal = mix(normalize(v_Normal), normalize(v_TBN * normal), u_Normal_Amount);

        vec3  detail = Sample3(u_Detail, uv, u_ST, 0);
        float     ao = Sample1(u_AO, uv, u_ST);

        ao = mix(1.0, 0.0, clamp((1.0 - ao) * u_AO_Amount, 0.0, 1.0));

        vec3 emission = Sample3(u_Emission, uv, u_ST) * u_Emission_Amount;

        /* DIRECT */

        vec3 directLighting;
        {
            float attenuation = Attenuation(u_LightPosition, fragPos, u_LightRange);

            float visibility = clamp(
                (
                    (dot(u_LightDirection, lightDir) > u_LightAngle ? 1.0 : 0.0) *
                    //1.0 - TransferShadow2D(v_Position_LightSpace, normal, lightDir, u_ShadowBias, u_ShadowNormalBias)
                    1.0 - max(
                        TransferShadow3D(normal, lightDir, v_Position, u_ShadowBias, u_ShadowNormalBias),
                        ParallaxShadowsHard(u_Displacement, (transpose(v_TBN) * lightDir), uv, u_ST, u_Displacement_Amount)
                    )
                ),
                0.0,
                1.0
            );

            //gl_FragColor = Sample4(u_ShadowMap3D, (v_Position - u_LightPosition));
            //return;

            vec3 lighting = BRDF(
                albedo.rgb + detail,
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

        vec3 indirectLighting;
        {
            // Figure out the direction of the ambient light
            vec3 ambientDir = reflect(-viewDir, normal);

            // Sample at max mip level for the diffuse.
            // TODO: Actual diffuse irradiance.
            vec3 diffuse = SampleAmbient(normal, 0.95);

            // Sample at variable mip level (determined by roughness) for specular.
            vec3 specular = SampleAmbient(ambientDir, roughness);

            vec3 F0 = mix(vec3(0.04), vec3(1.0), metallic);

            vec3 fresnel = Fresnel(F0, max(0.0, dot(normal, ambientDir)));

            //diffuse;//
            indirectLighting = (((diffuse * (1.0 - metallic)) * albedo.rgb) + (fresnel * specular));
        }

        //vec4(indirectLighting, 1);//
        gl_FragColor = vec4(((directLighting + indirectLighting) * ao) + emission, 1.0);
    }