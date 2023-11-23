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

        vec3 c = Sample3(u_Color, v_TexCoord);
        float w = (Sample1(u_Weights, v_TexCoord) - 0.5) * 2.0;

        // Less accurate than a luminosity calculation, but easier to do.
        float brightness = (c.r + c.g + c.b) / 3.0;

        gl_FragColor = vec4(pow(2.0, brightness)) * w;
    }