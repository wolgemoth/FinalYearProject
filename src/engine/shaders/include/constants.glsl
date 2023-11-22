#ifndef _CONSTANTS
#define _CONSTANTS

    const float F32PMAX =  340282346638528859811704183484516925440.0;
    const float F32NMAX = -340282346638528859811704183484516925440.0;

    const float F32PMIN =  0.00000000000000000000000000000000000001175494;
    const float F32NMIN = -0.00000000000000000000000000000000000001175494;

    const float KEPSILON = 0.005;

    const float PI  = 3.1415926535897932384626;
    const float PHI = 1.61803398874989484820458;
    const float E   = 2.71828182845904523536;

    const vec3 VEC3_NULL = vec3(0.0, 0.0, 0.0);
    const vec3 VEC3_ZERO = vec3(0.0, 0.0, 0.0);
    const vec3 VEC3_ONE  = vec3(1.0, 1.0, 1.0);
    const vec3 VEC3_ERR  = vec3(1.0, 0.0, 1.0);

    const vec4 VEC4_NULL = vec4(0.0, 0.0, 0.0, 0.0);
    const vec4 VEC4_ZERO = vec4(0.0, 0.0, 0.0, 1.0);
    const vec4 VEC4_ONE  = vec4(1.0, 1.0, 1.0, 1.0);
    const vec4 VEC4_ERR  = vec4(1.0, 0.0, 1.0, 1.0);

#endif