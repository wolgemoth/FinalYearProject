#pragma vertex

    #version 330 core

    in mediump vec3 a_Position;
    in mediump vec3 a_Normal;
    in mediump vec2 a_TexCoord;
    in mediump vec3 a_Tangent;
    in mediump vec3 a_Bitangent;

    out mediump vec4 v_Position;
    out mediump vec2 v_TexCoord;
    out mediump mat3 v_TBN;

    /* PARAMETERS */
    uniform mediump mat4 u_Projection;
    uniform mediump mat4 u_Model;
    uniform mediump mat4 u_View;

    void main() {

        // Perform perspective projection on model vertex:
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

        // Position in model space:
        v_Position = u_Projection * u_View * vec4(a_Position, 1.0);

        // Texture coordinates:
        v_TexCoord = a_TexCoord;

        // Compute TBN matrix:
        v_TBN = mat3(
            normalize(vec3(u_Model * vec4(a_Tangent,   0.0))),
            normalize(vec3(u_Model * vec4(a_Bitangent, 0.0))),
            normalize(vec3(u_Model * vec4(a_Normal,    0.0)))
        );
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/common_utils.inc"

    in mediump vec2 v_TexCoord;
    in mediump vec4 v_Position;
    in mediump mat3 v_TBN;

    uniform sampler2D u_Normals;
    uniform sampler2D u_TexCoord_gBuffer;

    uniform mediump float u_NormalAmount = 1.0;

    uniform mediump vec2 u_ScreenDimensions;

    void main() {

        // de Vries, J. (n.d.). LearnOpenGL - Normal Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Normal-Mapping [Accessed 15 Dec. 2023].

        mediump vec2 uv = Sample2(u_TexCoord_gBuffer, (gl_FragCoord.xy / u_ScreenDimensions));

        mediump vec3 normal = normalize((Sample3(u_Normals, uv) * 2.0) - 1.0);
        normal = mix(v_TBN[2], normalize(v_TBN * normal), u_NormalAmount);

        gl_FragColor = vec4(normal, 1.0);
    }