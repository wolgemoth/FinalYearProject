#ifndef _COMMON_UTILS
#define _COMMON_UTILS

    float sqr(in float _value) {
        return _value * _value;
    }

    float length2(in vec3 _A, in vec3 _B) {

        vec3 delta = _B - _A;

        return dot(delta, delta);
    }

    vec2 TransformCoord(sampler2D _texture, in vec2 _uv, in vec4 _st) {
        return (_uv + fract(_st.zw)) * _st.xy;
    }

    vec4 Sample4(sampler2D _texture, in vec2 _uv) {
        return texture(_texture, _uv);
    }

    vec4 Sample4(sampler2D _texture, in vec2 _uv, in int _lod) {
        return texture(_texture, _uv, _lod);
    }

    vec4 Sample4(sampler2D _texture, in vec2 _uv, in vec4 _st) {
        return texture(_texture, TransformCoord(_texture, _uv, _st));
    }

    vec4 Sample4(sampler2D _texture, in vec2 _uv, in vec4 _st, in int _lod) {
        return texture(_texture, TransformCoord(_texture, _uv, _st), _lod);
    }

    vec4 Sample4(samplerCube _texture, in vec3 _dir) {
        return texture(_texture, _dir);
    }

    vec4 Sample4(samplerCube _texture, in vec3 _dir, in int _lod) {
        return texture(_texture, _dir, _lod);
    }

    vec3 Sample3(sampler2D _texture, in vec2 _uv) {
        return texture(_texture, _uv).rgb;
    }

    vec3 Sample3(sampler2D _texture, in vec2 _uv, in int _lod) {
        return texture(_texture, _uv, _lod).rgb;
    }

    vec3 Sample3(sampler2D _texture, in vec2 _uv, in vec4 _st) {
        return texture(_texture, TransformCoord(_texture, _uv, _st)).rgb;
    }

    vec3 Sample3(sampler2D _texture, in vec2 _uv, in vec4 _st, in int _lod) {
        return texture(_texture, TransformCoord(_texture, _uv, _st), _lod).rgb;
    }

    vec3 Sample3(samplerCube _texture, in vec3 _dir) {
        return texture(_texture, _dir).rgb;
    }

    vec3 Sample3(samplerCube _texture, in vec3 _dir, in int _lod) {
        return texture(_texture, _dir, _lod).rgb;
    }

    vec2 Sample2(sampler2D _texture, in vec2 _uv) {
        return texture(_texture, _uv).rg;
    }

    vec2 Sample2(sampler2D _texture, in vec2 _uv, in int _lod) {
        return texture(_texture, _uv, _lod).rg;
    }

    vec2 Sample2(sampler2D _texture, in vec2 _uv, in vec4 _st) {
        return texture(_texture, TransformCoord(_texture, _uv, _st)).rg;
    }

    vec2 Sample2(sampler2D _texture, in vec2 _uv, in vec4 _st, in int _lod) {
        return texture(_texture, TransformCoord(_texture, _uv, _st), _lod).rg;
    }

    vec2 Sample2(samplerCube _texture, in vec3 _dir) {
        return texture(_texture, _dir).rg;
    }

    vec2 Sample2(samplerCube _texture, in vec3 _dir, in int _lod) {
        return texture(_texture, _dir, _lod).rg;
    }

    float Sample1(sampler2D _texture, in vec2 _uv) {
        return texture(_texture, _uv).r;
    }

    float Sample1(sampler2D _texture, in vec2 _uv, in int _lod) {
        return texture(_texture, _uv, _lod).r;
    }

    float Sample1(sampler2D _texture, in vec2 _uv, in vec4 _st) {
        return texture(_texture, TransformCoord(_texture, _uv, _st)).r;
    }

    float Sample1(sampler2D _texture, in vec2 _uv, in vec4 _st, in int _lod) {
        return texture(_texture, TransformCoord(_texture, _uv, _st), _lod).r;
    }

    float Sample1(samplerCube _texture, in vec3 _dir) {
        return texture(_texture, _dir).r;
    }

    float Sample1(samplerCube _texture, in vec3 _dir, in int _lod) {
        return texture(_texture, _dir, _lod).r;
    }

    vec2 ParallaxMapping(in sampler2D _displacement, in vec3 _viewDir, in vec2 _uv, in vec4 _st, in float _scale) {

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

        vec2  currentTexCoords     = _uv;
        float currentDepthMapValue = texture(_displacement, TransformCoord(_displacement, _uv, _st)).r;

        while(currentLayerDepth < currentDepthMapValue) {
            // shift texture coordinates along direction of P
            currentTexCoords -= deltaTexCoords;
            // get depthmap value at current texture coordinates
            currentDepthMapValue = texture(_displacement, TransformCoord(_displacement, currentTexCoords, _st)).r;
            // get depth of next layer
            currentLayerDepth += layerDepth;
        }

        // get texture coordinates before collision (reverse operations)
        vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

        // get depth after and before collision for linear interpolation
        float afterDepth  = currentDepthMapValue - currentLayerDepth;
        float beforeDepth = texture(_displacement, TransformCoord(_displacement, prevTexCoords, _st)).r - currentLayerDepth + layerDepth;

        // interpolation of texture coordinates
        float weight = afterDepth / (afterDepth - beforeDepth);
        vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

        return finalTexCoords;
    }

#endif