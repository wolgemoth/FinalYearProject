#ifndef _COMMON_UTILS
#define _COMMON_UTILS

    float sqr(in float _value) {
        return _value * _value;
    }

    float length2(vec3 _A, vec3 _B) {

        vec3 delta = _B - _A;

        return dot(delta, delta);
    }

    vec2 TransformCoord(sampler2D _texture, in vec2 _coord, in vec4 _scaleTranslate) {
        return (_coord + fract(_scaleTranslate.zw)) * _scaleTranslate.xy;
    }

    vec2 ParallaxMapping(in sampler2D _displacement, in vec3 _viewDir, in vec2 _texCoords, in vec4 _scaleTranslate, float _scale) {

        const float minLayers = 8.0;
        const float maxLayers = 32.0;
        float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), _viewDir), 0.0));

        // calculate the size of each layer
        float layerDepth = 1.0 / numLayers;

        // depth of current layer
        float currentLayerDepth = 0.0;

        // the amount to shift the texture coordinates per layer (from vector P)
        vec2 P = _viewDir.xy / _viewDir.z * _scale;
        vec2 deltaTexCoords = P / numLayers;

        vec2  currentTexCoords     = _texCoords;
        float currentDepthMapValue = texture(_displacement, TransformCoord(_displacement, _texCoords, _scaleTranslate)).r;

        while(currentLayerDepth < currentDepthMapValue) {
            // shift texture coordinates along direction of P
            currentTexCoords -= deltaTexCoords;
            // get depthmap value at current texture coordinates
            currentDepthMapValue = texture(_displacement, TransformCoord(_displacement, currentTexCoords, _scaleTranslate)).r;
            // get depth of next layer
            currentLayerDepth += layerDepth;
        }

        // get texture coordinates before collision (reverse operations)
        vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

        // get depth after and before collision for linear interpolation
        float afterDepth  = currentDepthMapValue - currentLayerDepth;
        float beforeDepth = texture(_displacement, TransformCoord(_displacement, prevTexCoords, _scaleTranslate)).r - currentLayerDepth + layerDepth;

        // interpolation of texture coordinates
        float weight = afterDepth / (afterDepth - beforeDepth);
        vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

        return finalTexCoords;
    }

#endif