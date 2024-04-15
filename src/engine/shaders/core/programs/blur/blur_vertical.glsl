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
    #extension GL_ARB_shading_language_include  : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    in mediump vec2 v_TexCoord;

    uniform sampler2D u_Texture;

    uniform mediump float u_Strength = 1.0;

    uniform mediump float u_Step = 2.0;

    /**
     * @brief Blurs an image vertically.
     *
     * This function applies a vertical blur to an input texture using the given coordinate and blur dispersion.
     *
     * @param[in] _coordinate The coordinates, as a 2-component vector, of the texture to be blurred.
     * @param[in] _diffusion The amount of dispersion in the blur effect.
     *
     * @returns The resulting 4-component vector (color) after applying blur.
     */
    vec4 BlurVertical(in vec2 _coordinate, const float _diffusion) {

        mediump vec2 l = (vec2(1.0) / textureSize(u_Texture, 0)) * _diffusion;

        mediump vec4 s =
            texture2D(u_Texture, v_TexCoord + vec2(0, -l.y)) +
            texture2D(u_Texture, v_TexCoord                ) +
            texture2D(u_Texture, v_TexCoord + vec2(0,  l.y));

        // Average.
        return s / 3.0;
    }

    void main() {

        mediump vec3 color = BlurVertical(v_TexCoord, u_Step).rgb;

        gl_FragColor = vec4(color, 1.0);
    }