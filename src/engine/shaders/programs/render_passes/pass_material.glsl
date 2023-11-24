#pragma vertex

    #version 330 core

    in vec3 a_Position;
    in vec3 a_Normal;
    in vec2 a_TexCoord;
    in vec3 a_Tangent;
    in vec3 a_Bitangent;

    out vec2 v_TexCoord;
    out vec3 v_Normal;

    /* PARAMETERS */
    uniform mat4 u_Projection;
    uniform mat4 u_Model;
    uniform mat4 u_View;

    void main() {

        // Perform perspective projection on model vertex:
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

        // Texture coordinates:
        v_TexCoord = a_TexCoord;

        // Normal:
        v_Normal = a_Normal;
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/common_utils.glsl"

    in vec2 v_TexCoord;
    in vec3 v_Normal;

    uniform sampler2D u_Roughness;
    uniform sampler2D u_Metallic;
    uniform sampler2D u_AO;

    uniform float u_Roughness_Amount = 1.0; // How rough the surface is.
    uniform float        u_AO_Amount = 1.0; // Strength of AO.

    uniform vec4 u_ST = vec4(1.0, 1.0, 0.0, 0.0);

    void main() {

        float roughness = Sample1(u_Roughness, v_TexCoord, u_ST) * u_Roughness_Amount;
        float  metallic = Sample1(u_Metallic,  v_TexCoord, u_ST);
        float        ao = Sample1(u_AO,        v_TexCoord, u_ST);

        ao = mix(1.0, 0.0, clamp((1.0 - ao) * u_AO_Amount, 0.0, 1.0));

        gl_FragColor = vec4(roughness, metallic, ao, 0.0);
    }