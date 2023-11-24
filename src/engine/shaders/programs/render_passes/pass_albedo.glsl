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

    uniform sampler2D u_Albedo;
    uniform sampler2D u_TexCoord_gBuffer;

    uniform vec2 u_ScreenDimensions;

    void main() {

        vec2 screen_uv = (gl_FragCoord.xy / u_ScreenDimensions);

        vec2 uv = Sample2(u_TexCoord_gBuffer, screen_uv);

        gl_FragColor = Sample4(u_Albedo, uv);
    }