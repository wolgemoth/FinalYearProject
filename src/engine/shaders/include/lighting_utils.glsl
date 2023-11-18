#ifndef _LIGHTING_UTILS
#define _LIGHTING_UTILS

    #include "/shaders/include/common_utils.glsl"

    // Light falloff with inverse square law.
    float Attenuation(vec3 _lightPosition, vec3 _fragPosition, float _range) {
        return _range / length2(_lightPosition, _fragPosition);
    }

#endif