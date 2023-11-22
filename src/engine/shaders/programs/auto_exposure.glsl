#pragma vertex

    #version 330 core

    in vec2 a_Position;
    in vec2 a_TexCoord;

    out vec2 v_TexCoord;

    void main() {

        v_TexCoord = a_TexCoord;

        gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/common_utils.glsl"

    in vec2 v_TexCoord;

    uniform sampler2D u_Color;
    uniform sampler2D u_Weights;

    /* PARAMETERS */

    void main() {
        gl_FragColor = vec4(Luma(Sample3(u_Color, v_TexCoord), 0)) * Sample1(u_Weights, v_TexCoord);
    }