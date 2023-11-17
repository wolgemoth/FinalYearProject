#ifndef _RAND
#define _RAND

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/constants.glsl"

    /* THIRD-PARTY */

    // Gold Noise ©2015 dcerisano@standard3d.com
    // - based on the Golden Ratio
    // - uniform normalized distribution
    // - fastest static noise generator function (also runs at low precision)
    // - use with indicated fractional seeding method.
    float gold_noise(in vec2 _xy, in float _seed) {
        return fract(tan(distance(_xy * PHI, _xy) * _seed) * _xy.x);
    }

    /* NOT THIRD-PARTY */

    float Random1(in vec2 _xy, in float _seed, in float _offset) {
        return (gold_noise(_xy * 1000.0, _seed + _offset) - 0.5) * 2.0;
    }

    vec2 Random2(in vec2 _xy, in float _seed, in float _offset) {

        return vec2(
            Random1(_xy, _seed, _offset + 0.1),
            Random1(_xy, _seed, _offset + 0.2)
        );
    }

    vec3 Random3(in vec3 _xyz, in float _seed, in float _offset) {

        return vec3(
            Random1(_xyz.xy, _seed, _offset + 0.1),
            Random1(_xyz.yz, _seed, _offset + 0.2),
            Random1(_xyz.xz, _seed, _offset + 0.3)
        );
    }

#endif