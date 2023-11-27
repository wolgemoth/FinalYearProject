#pragma vertex

    #version 330 core

    in vec3 a_Position;
    in vec3 a_Normal;
    in vec2 a_TexCoord;
    in vec3 a_Tangent;
    in vec3 a_Bitangent;

    out vec2 v_TexCoord;
    out vec3 v_FragPos_Tangent;
    out vec3 v_LightPos_Tangent;

    /* PARAMETERS */
    uniform mat4 u_Projection;
    uniform mat4 u_Model;
    uniform mat4 u_View;

    uniform vec3 u_LightPosition;

    void main() {

        // Perform perspective projection on model vertex:
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

        // Texture coordinates:
        v_TexCoord = a_TexCoord;

        // Compute (inverse) TBN matrix:
        mat3 tbn = transpose(mat3(
            normalize(vec3(u_Model * vec4(a_Tangent,   0))),
            normalize(vec3(u_Model * vec4(a_Bitangent, 0))),
            normalize(vec3(u_Model * vec4(transpose(inverse(mat3(u_Model))) * a_Normal, 0)))
        ));

        v_LightPos_Tangent = tbn * u_LightPosition;
         v_FragPos_Tangent = tbn * vec3(u_Model * vec4(a_Position, 1.0));
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/common_utils.glsl"
    #include "/shaders/include/lighting_utils.glsl"

    in vec2 v_TexCoord;
    in vec3 v_FragPos_Tangent;
    in vec3 v_LightPos_Tangent;

    uniform sampler2D u_Roughness;
    uniform sampler2D u_Metallic;
    uniform sampler2D u_AO;
    uniform sampler2D u_Displacement;

    /* G-BUFFER */
    uniform sampler2D u_TexCoord_gBuffer;

    uniform vec2 u_ScreenDimensions;

    uniform float    u_Roughness_Amount = 1.0; // How rough the surface is.
    uniform float           u_AO_Amount = 1.0; // Strength of AO.
    uniform float u_Displacement_Amount = 0.0; // Strength of displacement.

    void main() {

        vec2 uv = Sample2(u_TexCoord_gBuffer, gl_FragCoord.xy / u_ScreenDimensions);

        float roughness = Sample1(u_Roughness, uv) * u_Roughness_Amount;
        float  metallic = Sample1(u_Metallic,  uv);
        float        ao = Sample1(u_AO,        uv);

        ao = mix(1.0, 0.0, clamp((1.0 - ao) * u_AO_Amount, 0.0, 1.0));

        vec3 lightDir = normalize(v_LightPos_Tangent - v_FragPos_Tangent);

        float parallaxShadow = ParallaxShadowsHard(
            u_Displacement,
            lightDir,
            uv,
            vec4(1.0, 1.0, 0.0, 0.0),
            u_Displacement_Amount
        );

        gl_FragColor = vec4(roughness, metallic, ao, parallaxShadow);
    }