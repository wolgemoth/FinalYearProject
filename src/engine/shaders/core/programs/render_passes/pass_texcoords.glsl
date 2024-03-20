#pragma vertex

    #version 330 core

    in mediump vec3 a_Position;
    in mediump vec3 a_Normal;
    in mediump vec2 a_TexCoord;
    in mediump vec3 a_Tangent;
    in mediump vec3 a_Bitangent;

    out mediump vec2 v_TexCoord;
    out mediump vec3 v_FragPos_Tangent;
    out mediump vec3 v_CamPos_Tangent;

    /* PARAMETERS */
    uniform mediump mat4 u_Projection;
    uniform mediump mat4 u_Model;
    uniform mediump mat4 u_View;

    uniform mediump vec3 u_CameraPosition;

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

         v_CamPos_Tangent = tbn * u_CameraPosition;
        v_FragPos_Tangent = tbn * vec3(u_Model * vec4(a_Position, 1.0));
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/core/include/common_utils.glsl"

    in mediump vec2 v_TexCoord;
    in mediump vec3 v_FragPos_Tangent;
    in mediump vec3 v_CamPos_Tangent;

    uniform sampler2D u_Displacement;
    uniform mediump float u_Displacement_Amount = 0.0; // Strength of displacement.

    uniform mediump vec4 u_ST = vec4(1.0, 1.0, 0.0, 0.0);

    void main() {

        mediump vec3 viewDir_Tangent = normalize(v_CamPos_Tangent - v_FragPos_Tangent);

        mediump vec2 uv = ParallaxMapping(
            u_Displacement,
            viewDir_Tangent,
            v_TexCoord,
            u_ST,
            u_Displacement_Amount
        );

        gl_FragColor = vec4(TransformCoord(uv, u_ST), 0.0, 0.0);
    }