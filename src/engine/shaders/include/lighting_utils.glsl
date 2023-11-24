#ifndef _LIGHTING_UTILS
#define _LIGHTING_UTILS

    #include "/shaders/include/common_utils.glsl"

    // Light falloff with inverse square law.
    float Attenuation(vec3 _lightPosition, vec3 _fragPosition, float _range) {
        return _range / length2(_lightPosition, _fragPosition);
    }

    float ParallaxShadowsHard(in sampler2D _displacement, in vec3 _lightDir, in vec2 _uv, in vec4 _st, in float _scale) {

        float minLayers = 0;
        float maxLayers = 32;
        float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), _lightDir)));

        vec2 currentTexCoords = _uv;
        float currentDepthMapValue = Sample1(_displacement, currentTexCoords, _st);
        float currentLayerDepth = currentDepthMapValue;

        float layerDepth = 1.0 / numLayers;
        vec2 P = _lightDir.xy / _lightDir.z * _scale;
        vec2 deltaTexCoords = P / numLayers;

        while (currentLayerDepth <= currentDepthMapValue && currentLayerDepth > 0.0) {

            currentTexCoords += deltaTexCoords;
            currentDepthMapValue = Sample1(_displacement, currentTexCoords, _st);
            currentLayerDepth -= layerDepth;
        }

        return currentLayerDepth > currentDepthMapValue ? 1.0 : 0.0;
    }

#endif