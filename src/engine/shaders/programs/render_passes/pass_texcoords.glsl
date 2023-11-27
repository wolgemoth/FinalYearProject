#pragma vertex

    #version 330 core

    in vec3 a_Position;
    in vec3 a_Normal;
    in vec2 a_TexCoord;
    in vec3 a_Tangent;
    in vec3 a_Bitangent;

    out vec2 v_TexCoord;
    out vec3 v_FragPos_Tangent;
    out vec3 v_CamPos_Tangent;

    /* PARAMETERS */
    uniform mat4 u_Projection;
    uniform mat4 u_Model;
    uniform mat4 u_View;

    uniform vec3 u_CameraPosition;

    void main() {

        // Perform perspective projection on model vertex:
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

        // Texture coordinates:
        v_TexCoord = a_TexCoord;

        // Compute (Inverse) TBN matrix:
        mat3 tbn = transpose(mat3(
            normalize(vec3(u_Model * vec4(a_Tangent,   0))),
            normalize(vec3(u_Model * vec4(a_Bitangent, 0))),
            normalize(vec3(u_Model * vec4(transpose(inverse(mat3(u_Model))) * a_Normal, 0)))
        ));

         v_CamPos_Tangent = tbn * u_CameraPosition;
        v_FragPos_Tangent = tbn * vec3(u_Model * vec4(a_Position, 1.0));
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/common_utils.glsl"
    #include "/shaders/include/lighting_utils.glsl"

    in vec2 v_TexCoord;
    in vec3 v_FragPos_Tangent;
    in vec3 v_CamPos_Tangent;

    uniform sampler2D u_Displacement;
    uniform float u_Displacement_Amount = 0.0; // Strength of displacement.

    uniform vec4 u_ST = vec4(1.0, 1.0, 0.0, 0.0);

    void main() {

        vec3 viewDir_Tangent = normalize(v_CamPos_Tangent - v_FragPos_Tangent);

        vec2 uv = ParallaxMapping(
            u_Displacement,
            viewDir_Tangent,
            v_TexCoord,
            u_ST,
            u_Displacement_Amount
        );

        gl_FragColor = vec4(TransformCoord(uv, u_ST), 0.0, 0.0);
    }