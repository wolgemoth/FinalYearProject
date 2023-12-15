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

    #extension GL_ARB_texture_query_levels      : enable

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/common_utils.glsl"

    in mediump vec2 v_TexCoord;

    uniform sampler2D u_Texture;

    uniform mediump vec2 u_Diffusion;

    void main() {

        // @Assessor:
        // Jimenez, J. (n.d.). LearnOpenGL - Phys. Based Bloom. [online] learnopengl.com. Available at: https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom [Accessed 15 Dec. 2023].

        mediump vec2 size = textureSize(u_Texture, 0);

        // The filter kernel is applied with a radius, specified in texture
        // coordinates, so that the radius will vary across mip resolutions.
        mediump float x = u_Diffusion.x / size.x;
        mediump float y = u_Diffusion.y / size.y;

        // Take 9 samples around current texel:
        // a - b - c
        // d - e - f
        // g - h - i
        // === ('e' is the current texel) ===
        mediump vec3 a = Sample3(u_Texture, v_TexCoord + vec2( -x, y));
        mediump vec3 b = Sample3(u_Texture, v_TexCoord + vec2(0.0, y));
        mediump vec3 c = Sample3(u_Texture, v_TexCoord + vec2(  x, y));

        mediump vec3 d = Sample3(u_Texture, v_TexCoord + vec2( -x, 0.0));
        mediump vec3 e = Sample3(u_Texture, v_TexCoord + vec2(0.0, 0.0));
        mediump vec3 f = Sample3(u_Texture, v_TexCoord + vec2(  x, 0.0));

        mediump vec3 g = Sample3(u_Texture, v_TexCoord + vec2( -x, -y));
        mediump vec3 h = Sample3(u_Texture, v_TexCoord + vec2(0.0, -y));
        mediump vec3 i = Sample3(u_Texture, v_TexCoord + vec2(  x, -y));

        // Apply weighted distribution, by using a 3x3 tent filter:
        //  1   | 1 2 1 |
        // -- * | 2 4 2 |
        // 16   | 1 2 1 |
        mediump vec3 col = e * 4.0;
        col += (b + d + f + h) * 2.0;
        col += (a + c + g + i);
        col *= 1.0 / 16.0;

        gl_FragColor = vec4(col, 1.0);
    }