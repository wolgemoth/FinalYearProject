#pragma vertex

    #version 330 core

    in mediump vec3 a_Position;
    in mediump vec2 a_TexCoord;

    out mediump vec2 v_TexCoord;

    /* PARAMETERS */
    uniform mediump mat4 u_Projection;
    uniform mediump mat4 u_Model;
    uniform mediump mat4 u_View;

    void main() {

        // Perform perspective projection on model vertex:
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

        // Texture coordinates:
        v_TexCoord = a_TexCoord;
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/common_utils.inc"

    in mediump vec2 v_TexCoord;

    uniform sampler2D u_Emission;
    uniform sampler2D u_TexCoord_gBuffer;

    uniform mediump vec2 u_ScreenDimensions;

    uniform vec3 u_EmissionColor;

    void main() {

        mediump vec2 uv = Sample2(u_TexCoord_gBuffer, (gl_FragCoord.xy / u_ScreenDimensions));

        gl_FragColor = vec4(Sample3(u_Emission, uv) * u_EmissionColor, 1.0);
    }