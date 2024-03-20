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

    #extension GL_ARB_texture_query_levels     : enable
    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/common_utils.glsl"

    in mediump vec2 v_TexCoord;

    uniform sampler2D u_Texture0;
    uniform sampler2D u_Dirt;
    uniform sampler2D u_Bloom;

    /* PARAMETERS */
    uniform mediump float u_Strength;

    void main() {

        mediump vec3   dst_col = Sample3(u_Texture0, v_TexCoord);
        mediump vec3  dirt_col = Sample3(u_Dirt, v_TexCoord);
        mediump vec3 bloom_col = Sample3(u_Bloom, v_TexCoord);

        // Modulate the brightness of the dirt color using the bloom color, and add to the original color.
        gl_FragColor = vec4(dst_col + (dirt_col * (bloom_col * u_Strength)), 1.0);
    }