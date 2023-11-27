#pragma vertex

    #version 330 core

    in vec3 a_Position;
    in vec3 a_Normal;
    in vec2 a_TexCoord;
    in vec3 a_Tangent;
    in vec3 a_Bitangent;

    out vec4 v_Position;
    out vec2 v_TexCoord;
    out vec3 v_Normal;
    out mat3 v_TBN;

    /* PARAMETERS */
    uniform mat4 u_Projection;
    uniform mat4 u_Model;
    uniform mat4 u_View;

    void main() {

        // Perform perspective projection on model vertex:
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

        // Position in model space:
        v_Position = u_Projection * u_View * vec4(a_Position, 1.0);

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
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/common_utils.glsl"

    in vec2 v_TexCoord;
    in vec4 v_Position;
    in vec3 v_Normal;
    in mat3 v_TBN;

    uniform sampler2D u_Normals;
    uniform sampler2D u_TexCoord_gBuffer;

    uniform float u_Normal_Amount = 1.0;

    uniform vec2 u_ScreenDimensions;

    void main() {

        vec2 uv = Sample2(u_TexCoord_gBuffer, (gl_FragCoord.xy / u_ScreenDimensions));

        vec3 normal = normalize((Sample3(u_Normals, uv) * 2.0) - 1.0);
        normal = mix(v_Normal, normalize(v_TBN * normal), u_Normal_Amount);

        gl_FragColor = vec4(normal, 1.0f);
    }