#pragma vertex

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

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

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    #include "/assets/shaders/core/include/common_utils.inc"

    in mediump vec2 v_TexCoord;

    uniform sampler2D u_Texture;

    void main() {

        // Jimenez, J. (n.d.). LearnOpenGL - Phys. Based Bloom. [online] learnopengl.com. Available at: https://learnopengl.com/Guest-Articles/2022/Phys.-Based-Bloom [Accessed 15 Dec. 2023].

        mediump vec2 texelSize = vec2(1.0) / textureSize(u_Texture, 0);
        mediump float x = texelSize.x;
        mediump float y = texelSize.y;

        // Take 13 samples around current texel:
        // a - b - c
        // - j - k -
        // d - e - f
        // - l - m -
        // g - h - i
        // === ('e' is the current texel) ===
        mediump vec3 a = Sample3(u_Texture, v_TexCoord + vec2(-2.0 * x, 2.0 * y));
        mediump vec3 b = Sample3(u_Texture, v_TexCoord + vec2( 0.0,     2.0 * y));
        mediump vec3 c = Sample3(u_Texture, v_TexCoord + vec2( 2.0 * x, 2.0 * y));

        mediump vec3 d = Sample3(u_Texture, v_TexCoord + vec2(-2.0 * x, 0.0));
        mediump vec3 e = Sample3(u_Texture, v_TexCoord + vec2( 0.0,     0.0));
        mediump vec3 f = Sample3(u_Texture, v_TexCoord + vec2( 2.0 * x, 0.0));

        mediump vec3 g = Sample3(u_Texture, v_TexCoord + vec2(-2.0 *x, -2.0 * y));
        mediump vec3 h = Sample3(u_Texture, v_TexCoord + vec2( 0.0,    -2.0 * y));
        mediump vec3 i = Sample3(u_Texture, v_TexCoord + vec2( 2.0 *x, -2.0 * y));

        mediump vec3 j = Sample3(u_Texture, v_TexCoord + vec2(-x,  y));
        mediump vec3 k = Sample3(u_Texture, v_TexCoord + vec2( x,  y));
        mediump vec3 l = Sample3(u_Texture, v_TexCoord + vec2(-x, -y));
        mediump vec3 m = Sample3(u_Texture, v_TexCoord + vec2( x, -y));

        // Apply weighted distribution:
        // 0.5 + 0.125 + 0.125 + 0.125 + 0.125 = 1
        // a,b,d,e * 0.125
        // b,c,e,f * 0.125
        // d,e,g,h * 0.125
        // e,f,h,i * 0.125
        // j,k,l,m * 0.5
        // This shows 5 square areas that are being sampled. But some of them overlap,
        // so to have an energy preserving downsample we need to make some adjustments.
        // The weights are the distributed, so that the sum of j,k,l,m (e.g.)
        // contribute 0.5 to the final color output. The code below is written
        // to effectively yield this sum. We get:
        // 0.125*5 + 0.03125*4 + 0.0625*4 = 1
        mediump vec3 col = e * 0.125;
        col += (a + c + g + i) * 0.03125;
        col += (b + d + f + h) * 0.0625;
        col += (j + k + l + m) * 0.125;

        gl_FragColor = vec4(col, 1.0);
    }