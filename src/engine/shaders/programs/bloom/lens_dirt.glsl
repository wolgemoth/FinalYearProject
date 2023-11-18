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
    uniform sampler2D u_Dirt;
    uniform sampler2D u_Bloom;

    /* PARAMETERS */
    uniform float u_Strength;

    void main() {

        vec3 dst_col = texture2D(u_Texture0, v_TexCoord).rgb;
        vec3 dirt_col = texture2D(u_Dirt, v_TexCoord).rgb;
        vec3 bloom_col = texture2D(u_Bloom, v_TexCoord).rgb;

        gl_FragColor = vec4(dst_col + (dirt_col * (bloom_col * u_Strength)), 1.0);
    }