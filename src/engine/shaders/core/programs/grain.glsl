#pragma vertex

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    in mediump vec2 a_Position;
    in mediump vec2 a_TexCoord;

    out mediump vec2 v_TexCoord;

    void main() {

        v_TexCoord = a_TexCoord;

        gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    #include "/assets/shaders/core/include/rand.inc"
    #include "/assets/shaders/core/include/common_utils.inc"

    in mediump vec2 v_TexCoord;

    uniform sampler2D u_Texture;

    /* PARAMETERS */
    uniform mediump float u_Amount;
    uniform mediump float u_Time;

    void main() {

        mediump vec3 grain = Random3S(vec3(v_TexCoord.x, v_TexCoord.y, v_TexCoord.x + v_TexCoord.y), u_Time, 0.5);

        gl_FragColor = vec4(Sample3(u_Texture, v_TexCoord) - (grain * u_Amount), 1.0);
    }