#pragma vertex

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    in mediump vec3 a_Position;
    in mediump vec3 a_Normal;
    in mediump vec2 a_TexCoord;
    in mediump vec3 a_Tangent;
    in mediump vec3 a_Bitangent;

    out mediump vec2 v_TexCoord;
    out mediump vec3 v_FragPos_Tangent;
    out mediump vec3 v_LightPos_Tangent;

    /* PARAMETERS */
    uniform mediump mat4 u_Projection;
    uniform mediump mat4 u_Model;
    uniform mediump mat4 u_View;

    uniform mediump vec3 u_LightPosition;

    void main() {

        // Perform perspective projection on model vertex:
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

        // Texture coordinates:
        v_TexCoord = a_TexCoord;

        // Compute (Inverse) TBN matrix:
        mediump mat3 tbn = transpose(mat3(
            normalize(vec3(u_Model * vec4(a_Tangent,   0.0))),
            normalize(vec3(u_Model * vec4(a_Bitangent, 0.0))),
            normalize(vec3(u_Model * vec4(transpose(inverse(mat3(u_Model))) * a_Normal, 0.0)))
        ));

        v_LightPos_Tangent = tbn * u_LightPosition;
         v_FragPos_Tangent = tbn * vec3(u_Model * vec4(a_Position, 1.0));
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    #include "/assets/shaders/core/include/common_utils.inc"
    #include "/assets/shaders/core/include/lighting_utils.inc"

    in mediump vec2 v_TexCoord;
    in mediump vec3 v_FragPos_Tangent;
    in mediump vec3 v_LightPos_Tangent;

    uniform sampler2D u_Roughness;
    uniform sampler2D u_Metallic;
    uniform sampler2D u_AO;
    uniform sampler2D u_Displacement;

    /* G-BUFFER */
    uniform sampler2D u_TexCoord_gBuffer;

    uniform mediump vec2 u_ScreenDimensions;

    uniform bool u_ParallaxShadows = false;

    uniform mediump float    u_Roughness_Amount = 1.0; // How rough the surface is.
    uniform mediump float           u_AO_Amount = 1.0; // Strength of AO.
    uniform mediump float u_Displacement_Amount = 0.0; // Strength of displacement.

    void main() {

        mediump vec2 uv = Sample2(u_TexCoord_gBuffer, gl_FragCoord.xy / u_ScreenDimensions);

        mediump float roughness = Sample1(u_Roughness, uv) * u_Roughness_Amount;
        mediump float  metallic = Sample1(u_Metallic,  uv);
        mediump float        ao = Sample1(u_AO,        uv);

        ao = mix(1.0, 0.0, clamp((1.0 - ao) * u_AO_Amount, 0.0, 1.0));

        mediump vec3 lightDir_Tangent = normalize(v_LightPos_Tangent - v_FragPos_Tangent);

        const float displacement_multiplier = 1.0;

        mediump float parallaxShadow = u_ParallaxShadows ?
            ParallaxShadowsHard(
                u_Displacement,
                lightDir_Tangent,
                uv,
                vec4(1.0, 1.0, 0.0, 0.0),
                u_Displacement_Amount
            ) :
            0.0;

        gl_FragColor = vec4(roughness, metallic, ao, parallaxShadow);
    }