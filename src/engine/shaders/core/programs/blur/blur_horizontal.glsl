#pragma vertex

    #version 330 core

    layout (location = 0) in mediump vec2 a_Position;
    layout (location = 1) in mediump vec2 a_TexCoord;

    out mediump vec2 v_TexCoord;

    void main() {

        v_TexCoord = a_TexCoord;

        gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_explicit_uniform_location : enable

    in mediump vec2 v_TexCoord;

    uniform sampler2D u_Texture;

    uniform mediump float u_Strength = 1.0;

    uniform mediump float u_Step = 2.0;

    /// <summary> Simple box blur horizontal pass. </summary>
    vec4 BlurHorizontal(in vec2 _coordinate, const float _diffusion) {

        mediump vec2 l = (vec2(1.0) / textureSize(u_Texture, 0)) * _diffusion;

        mediump vec4 s =
            texture2D(u_Texture, v_TexCoord + vec2(-l.x, 0)) +
            texture2D(u_Texture, v_TexCoord                ) +
            texture2D(u_Texture, v_TexCoord + vec2( l.x, 0));

        // Average.
        return s / 3.0;
    }

    void main() {

        mediump vec3 color = BlurHorizontal(v_TexCoord, u_Step).rgb;

        gl_FragColor = vec4(color, 1.0);
    }