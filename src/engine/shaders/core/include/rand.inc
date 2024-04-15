#ifndef _RAND
#define _RAND

    #include "/assets/shaders/core/include/constants.inc"

    /* THIRD-PARTY */

    // Cerisano, D. (2022). Random / noise functions for GLSL. [online] Stack Overflow. Available at: https://stackoverflow.com/questions/4200224/random-noise-functions-for-glsl [Accessed 15 Dec. 2023].

    // Gold Noise ©2015 dcerisano@standard3d.com
    // - based on the Golden Ratio
    // - uniform normalized distribution
    // - fastest static noise generator function (also runs at low precision)
    // - use with indicated fractional seeding method.
    float gold_noise(in vec2 _xy, in float _seed) {
        return fract(tan(distance(_xy * PHI, _xy) * _seed) * _xy.x);
    }

    /* NOT THIRD-PARTY */

    /* SIGNED */

    /*!
     * \brief Random Noise. 1-Dimensional. Signed.
     *
     * This function uses Golden Ratio Noise function to generate random values with a 2D vector as input, a seed and an offset.
     *
     * \param[in] _xy Vector2 input to the Golden Noise function.
     * \param[in] _seed Float value for seeding the Golden Noise function.
     * \param[in] _offset Float value offset for the Golden Noise function.
     *
     * \return random float value between -1 and 1
     */
    float Random1S(in vec2 _xy, in float _seed, in float _offset) {
        return clamp(
            (gold_noise(_xy * 1000.0, _seed + _offset) - 0.5) * 2.0,
            -1.0,
             1.0
        );
    }

    /*!
     * \brief Random Noise. 2-Dimensional. Signed.
     *
     * This function uses Golden Ratio Noise function to generate random values with a 2D vector as input, a seed and an offset.
     *
     * \param[in] _xy Vector2 input to the Golden Noise function.
     * \param[in] _seed Float value for seeding the Golden Noise function.
     * \param[in] _offset Float value offset for the Golden Noise function.
     *
     * \return Vector of random float values between -1 and 1
     */
    vec2 Random2S(in vec2 _xy, in float _seed, in float _offset) {

        return vec2(
            Random1S(_xy, _seed, _offset + 0.1),
            Random1S(_xy, _seed, _offset + 0.2)
        );
    }

    /*!
     * \brief Random Noise. 3-Dimensional. Signed.
     *
     * This function uses Golden Ratio Noise function to generate random values with a 2D vector as input, a seed and an offset.
     *
     * \param[in] _xy Vector2 input to the Golden Noise function.
     * \param[in] _seed Float value for seeding the Golden Noise function.
     * \param[in] _offset Float value offset for the Golden Noise function.
     *
     * \return Vector of random float values between -1 and 1
     */
    vec3 Random3S(in vec3 _xyz, in float _seed, in float _offset) {

        return vec3(
            Random1S(_xyz.xy, _seed, _offset + 0.1),
            Random1S(_xyz.yz, _seed, _offset + 0.2),
            Random1S(_xyz.xz, _seed, _offset + 0.3)
        );
    }

    /* UNSIGNED */

    /*!
     * \brief Random Noise. 1-Dimensional. Unigned.
     *
     * This function uses Golden Ratio Noise function to generate random values with a 2D vector as input, a seed and an offset.
     *
     * \param[in] _xy Vector2 input to the Golden Noise function.
     * \param[in] _seed Float value for seeding the Golden Noise function.
     * \param[in] _offset Float value offset for the Golden Noise function.
     *
     * \return random float value between 0 and 1
     */
    float Random1U(in vec2 _xy, in float _seed, in float _offset) {
        return clamp(
            gold_noise(_xy * 1000.0, _seed + _offset),
            0.0,
            1.0
        );
    }
/*!
     * \brief Random Noise. 2-Dimensional. Unigned.
     *
     * This function uses Golden Ratio Noise function to generate random values with a 2D vector as input, a seed and an offset.
     *
     * \param[in] _xy Vector2 input to the Golden Noise function.
     * \param[in] _seed Float value for seeding the Golden Noise function.
     * \param[in] _offset Float value offset for the Golden Noise function.
     *
     * \return Vector of random float values between 0 and 1
     */
    vec2 Random2U(in vec2 _xy, in float _seed, in float _offset) {

        return vec2(
            Random1U(_xy, _seed, _offset + 0.1),
            Random1U(_xy, _seed, _offset + 0.2)
        );
    }
/*!
     * \brief Random Noise. 3-Dimensional. Unigned.
     *
     * This function uses Golden Ratio Noise function to generate random values with a 2D vector as input, a seed and an offset.
     *
     * \param[in] _xy Vector2 input to the Golden Noise function.
     * \param[in] _seed Float value for seeding the Golden Noise function.
     * \param[in] _offset Float value offset for the Golden Noise function.
     *
     * \return Vector of random float values between 0 and 1
     */
    vec3 Random3U(in vec3 _xyz, in float _seed, in float _offset) {

        return vec3(
            Random1U(_xyz.xy, _seed, _offset + 0.1),
            Random1U(_xyz.yz, _seed, _offset + 0.2),
            Random1U(_xyz.xz, _seed, _offset + 0.3)
        );
    }

#endif