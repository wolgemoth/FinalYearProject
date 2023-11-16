#pragma vertex

    #version 330 core

    layout (location = 0) in vec2 a_Position;
    layout (location = 1) in vec2 a_TexCoord;

    out vec2 v_TexCoord;

    void main() {

      v_TexCoord = a_TexCoord;

      gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    }

#pragma fragment

    #version 330 core

    uniform sampler2D u_Texture0;
    uniform sampler2D u_Texture1;

    in vec2 v_TexCoord;

    /* PARAMETERS */
    uniform float u_Strength;

    void main() {
        gl_FragColor = texture2D(u_Texture0, v_TexCoord) + (texture2D(u_Texture1, v_TexCoord) * u_Strength);
    }