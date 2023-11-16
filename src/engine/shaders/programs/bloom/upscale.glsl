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

    uniform sampler2D u_Texture;

    uniform vec2 u_Diffusion;

    void main() {

        vec2 size = textureSize(u_Texture, 0);

        // The filter kernel is applied with a radius, specified in texture
        // coordinates, so that the radius will vary across mip resolutions.
        float x = u_Diffusion.x / size.x;
        float y = u_Diffusion.y / size.y;

        // Take 9 samples around current texel:
        // a - b - c
        // d - e - f
        // g - h - i
        // === ('e' is the current texel) ===
        vec3 a = texture(u_Texture, v_TexCoord + vec2(-x,  y)).rgb;
        vec3 b = texture(u_Texture, v_TexCoord + vec2( 0,  y)).rgb;
        vec3 c = texture(u_Texture, v_TexCoord + vec2( x,  y)).rgb;

        vec3 d = texture(u_Texture, v_TexCoord + vec2(-x,  0)).rgb;
        vec3 e = texture(u_Texture, v_TexCoord + vec2( 0,  0)).rgb;
        vec3 f = texture(u_Texture, v_TexCoord + vec2( x,  0)).rgb;

        vec3 g = texture(u_Texture, v_TexCoord + vec2(-x, -y)).rgb;
        vec3 h = texture(u_Texture, v_TexCoord + vec2( 0, -y)).rgb;
        vec3 i = texture(u_Texture, v_TexCoord + vec2( x, -y)).rgb;

        // Apply weighted distribution, by using a 3x3 tent filter:
        //  1   | 1 2 1 |
        // -- * | 2 4 2 |
        // 16   | 1 2 1 |
        vec3 col = e*4.0;
        col += (b+d+f+h)*2.0;
        col += (a+c+g+i);
        col *= 1.0 / 16.0;

        gl_FragColor = vec4(col, 1);
    }