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

    in vec2 v_TexCoord;

    uniform sampler2D u_Texture0;

    /* PARAMETERS */

    float Luma(vec3 _color) {

        // https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color

        //return dot(_color, vec3(0.2126, 0.7152, 0.0722));
        return dot(_color, vec3(0.299, 0.587, 0.114));
        //return sqrt(dot(dot(_color, _color), (0.299, 0.587, 0.114)));
    }

    void main() {
        gl_FragColor = vec4(vec3(Luma(texture(u_Texture0, v_TexCoord).rgb)), 1);
    }