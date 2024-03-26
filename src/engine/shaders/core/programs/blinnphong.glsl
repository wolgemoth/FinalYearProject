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

    #include "/assets/shaders/core/include/rand.inc"
    #include "/assets/shaders/core/include/constants.inc"
    #include "/assets/shaders/core/include/common_utils.inc"
    #include "/assets/shaders/core/include/lighting_utils.inc"

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

    const mediump float PCSS_SCENE_SCALE = 0.0003125; // Scale of shadow blur (PCSS only).

    uniform mediump float u_ShadowBias       = 0.005; // Shadow bias.
    uniform mediump float u_ShadowNormalBias = 0.1;   // Shadow normal bias.
    uniform mediump float u_NearPlane        = 0.1;   // Light's shadow near plane.

    uniform int u_ShadowTechnique = 0;
    uniform int u_ShadowSamples = 10; // Number of shadow samples. Please choose a sane value.

    uniform mediump float u_Time;

    /* DIRECT LIGHTING */
    uniform int u_LightType = 0;

    uniform mediump  vec3 u_LightPosition;  // Position of light in world-space.
    uniform mediump  vec3 u_LightDirection; // Direction of the light in world-space.
    uniform mediump float u_LightRange;     // Range of light.
    uniform mediump float u_LightIntensity; // Brightness of light.
    uniform mediump  vec3 u_LightColor;     // Color of light.
    uniform mediump float u_LightSize       =  0.2; // Size of the light (PCSS only).
    uniform mediump float u_LightAngle      = -1.0; // Cos of light's FOV (for spot lights).

    mediump float Lambert(const vec3 _normal, const vec3 _lightDirection) {
        return max(dot(_normal, _lightDirection), 0.0);
    }

    mediump float BlinnPhong(const mediump vec3 _normal, const mediump vec3 _lightDirection, const mediump vec3 _viewDirection, in mediump float _roughness) {

        mediump vec3  halfDir   = normalize(_lightDirection + _viewDirection);
        mediump float specAngle = max(dot(halfDir, _normal), 0.0);

        mediump float specularity = clamp(_roughness, 0.0, 0.998);
        specularity *= specularity;

        return pow(specAngle, 1.0 / specularity);
    }

    mediump float TransferShadow3D(in mediump vec3 _normal, in mediump vec3 _lightDir, in mediump vec3 _fragPos, in mediump float _bias, in mediump float _normalBias) {

        mediump float texelSize = 1.0 /
            float(max(textureSize(u_ShadowMap3D, 0).x, 1));

        mediump vec3 fragToLight = _fragPos - u_LightPosition;

        mediump float bias_multiplier = 5000.0;

        mediump float adjustedBias =
            texelSize * bias_multiplier * max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

        float result;

        switch(u_ShadowTechnique) {

            case 0: {
                result = ShadowCalculationHard3D(u_ShadowMap3D, fragToLight, vec3(0.0), adjustedBias, u_LightRange);
                break;
            }
            case 1: {
                result = ShadowCalculationPCF3D(u_ShadowMap3D, fragToLight, texelSize, adjustedBias, 1.0f, u_LightRange);
                break;
            }
            case 2: {
                result = ShadowCalculationDisk3D(u_ShadowMap3D, fragToLight, texelSize, adjustedBias, 1.0f, u_LightRange, fract(u_Time), u_ShadowSamples);
                break;
            }
            case 3: {
                result = ShadowCalculationPCSS3D(u_ShadowMap3D, fragToLight, PCSS_SCENE_SCALE, adjustedBias, u_LightRange, u_LightSize, fract(u_Time), u_ShadowSamples);
                break;
            }
            default: {
                result = 0.0;
                break;
            }
        }

        return result;
    }

    mediump float TransferShadow2D(in mediump vec4 _fragPosLightSpace, in mediump vec3 _normal, in mediump vec3 _lightDir, in mediump float _bias, in mediump float _normalBias) {

        mediump float texelSize = 1.0 /
            float(max(textureSize(u_ShadowMap2D, 0).x, 1));

        mediump vec3 projCoords =
            ((_fragPosLightSpace.xyz / _fragPosLightSpace.w) * 0.5) + 0.5;

        mediump float        bias_multiplier = u_LightAngle == -1.0 ?    1.0 : 100.0;
        mediump float perspective_multiplier = u_LightAngle == -1.0 ? 1000.0 :   1.0;

        mediump float adjustedBias =
            texelSize * bias_multiplier * max(_normalBias * (1.0 - dot(_normal, _lightDir)), _bias);

        float result;

        switch(u_ShadowTechnique) {

            case 0: {
                result = ShadowCalculationHard2D(u_ShadowMap2D, projCoords, vec2(0.0), adjustedBias);
                break;
            }
            case 1: {
                result = ShadowCalculationPCF2D(u_ShadowMap2D, projCoords, texelSize, adjustedBias, 1.0f);
                break;
            }
            case 2: {
                result = ShadowCalculationDisk2D(u_ShadowMap2D, projCoords, texelSize, adjustedBias, 1.0f, fract(u_Time), u_ShadowSamples);
                break;
            }
            case 3: {
                result = ShadowCalculationPCSS2D(u_ShadowMap2D, projCoords, pow(PCSS_SCENE_SCALE, 2.0) * 2.0, adjustedBias, u_NearPlane, u_LightAngle, u_LightSize * perspective_multiplier, fract(u_Time), u_ShadowSamples);
                break;
            }
            default: {
                result = 0.0;
                break;
            }
        }

        return result;
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

            mediump float visibility;

            switch (u_LightType) {
                case 0: { /* POINT */
                    visibility = (dot(u_LightDirection, lightDir) > u_LightAngle ? 1.0 : 0.0) *
                    1.0 - max(
                        TransferShadow3D(normal, lightDir, position, u_ShadowBias, u_ShadowNormalBias),
                        ps
                    );
                    break;
                }
                case 1: {  /* DIRECTIONAL */
                    visibility = (dot(u_LightDirection, u_LightDirection) > u_LightAngle ? 1.0 : 0.0) *
                    1.0 - max(
                        TransferShadow2D(position_lightSpace, normal, u_LightDirection, u_ShadowBias, u_ShadowNormalBias),
                        ps
                    );
                    break;
                }
                case 2: { /* SPOT */
                    visibility = (dot(u_LightDirection, lightDir) > u_LightAngle ? 1.0 : 0.0) *
                    1.0 - max(
                        TransferShadow2D(position_lightSpace, normal, lightDir, u_ShadowBias, u_ShadowNormalBias),
                        ps
                    );
                    break;
                }
                default: {
                    visibility = 1.0;
                    break;
                }
            }

            visibility = clamp(visibility, 0.0, 1.0);

            mediump float lighting;

            if (u_LightType == 1) {

                lighting = (
                    Lambert(normal, u_LightDirection) +
                    BlinnPhong(normal, u_LightDirection, viewDir, roughness)
                );
            }
            else {

                lighting = (
                    Lambert(normal, lightDir) +
                    BlinnPhong(normal, lightDir, viewDir, roughness)
                ) * attenuation;
            }

            directLighting += (visibility * lighting * u_LightIntensity) * u_LightColor;
        }

        directLighting *= albedo.rgb;

        /* INDIRECT */

        mediump vec3 indirectLighting;
        {
            // Sample at a lower resolution for the diffuse. TODO: Actual diffuse irradiance.
            mediump vec3 diffuse = SampleAmbient(u_Ambient, normal, 1.0);

            // Figure out the direction of the specular light
            mediump vec3 specularDir = reflect(-viewDir, normal);

            // Sample at variable mip level (determined by roughness) for specular.
            mediump vec3 specular = SampleAmbient(u_Ambient, specularDir, roughness);

            mediump float fresnel = pow(1.0 - max(0.0, dot(normal, specularDir)), 5.0);

            indirectLighting = (((diffuse * (1.0 - metallic)) * albedo.rgb) + (fresnel * specular)) * u_AmbientExposure;
        }

        gl_FragColor = vec4(((directLighting + indirectLighting) * ao) + emission, 1.0);
    }