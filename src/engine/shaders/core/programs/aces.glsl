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

    #extension GL_ARB_explicit_uniform_location : enable
    #extension GL_ARB_texture_query_levels      : enable

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    #include "/assets/shaders/core/include/common_utils.inc"

    in mediump vec2 v_TexCoord;

    uniform sampler2D u_Texture;

    /* PARAMETERS */
    uniform mediump float u_Gain     = 0.0;
    uniform mediump float u_Exposure = 1.0;

    // Performs ACES tonemapping on a color represented by a 3D vector.
    // Taylor, M. (2019). Tone Mapping. [online] 64.github.io. Available at: https://64.github.io/tonemapping/ [Accessed 15 Dec. 2023].
    vec3 ACES(in vec3 _hdr) {

        mediump mat3 aInput;
        mediump mat3 aOutput;

        // ACES INPUT MAT
        aInput[0] = vec3(0.59719, 0.35458, 0.04823);
        aInput[1] = vec3(0.07600, 0.90834, 0.01566);
        aInput[2] = vec3(0.02840, 0.13383, 0.83777);

        _hdr = _hdr * aInput;

        // RTT ODT FIT
        mediump vec3 a = _hdr * (_hdr + 0.0245786) - 0.000090537;
        mediump vec3 b = _hdr * (0.983729 * _hdr + 0.4329510) + 0.238081;
        _hdr = a / b;

        // ACES OUTPUT MAT
        aOutput[0] = vec3( 1.60475, -0.53108, -0.07367);
        aOutput[1] = vec3(-0.10208,  1.10813, -0.00605);
        aOutput[2] = vec3(-0.00327, -0.07276,  1.07602);

        return _hdr * aOutput;
    }

    void main() {

        mediump vec3 tonemapped = ACES(max((Sample3(u_Texture, v_TexCoord) * u_Exposure) + u_Gain, vec3(0.0)));

        gl_FragColor = vec4(tonemapped, 1.0);
    }