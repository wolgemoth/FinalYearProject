#pragma vertex

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    /* PARAMETERS */
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
    #extension GL_ARB_texture_query_levels      : enable

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    #include "/assets/shaders/core/include/common_utils.inc"

    uniform sampler2D u_Texture;

    in mediump vec2 v_TexCoord;

    void main() {
        gl_FragColor = Sample4(u_Texture, v_TexCoord);
    }