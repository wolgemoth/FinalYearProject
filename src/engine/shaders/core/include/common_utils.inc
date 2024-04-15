#ifndef _COMMON_UTILS
#define _COMMON_UTILS

    #include "/assets/shaders/core/include/constants.inc"
/**
     * @brief Returns the square of a value.
     *
     * @param[in] _value The input value to be squared.
     *
     * @return The square of the input value.
     */
    int sqr(in int _value) {
        return _value * _value;
    }

    /**
     * @brief Returns the square of a value.
     *
     * @param[in] _value The input value to be squared.
     *
     * @return The square of the input value.
     */
    float sqr(in float _value) {
        return _value * _value;
    }

    /**
     * @brief Returns the square distance between two vectors.
     *
     * This function calculates the squared Euclidean distance between two points _A and _B.
     *
     * @param[in] _A The first point.
     * @param[in] _B The second point.
     *
     * @return The squared length (distance) between the two points _A and _B.
     */
    float length2(in vec3 _A, in vec3 _B) {

        vec3 delta = _B - _A;

        return dot(delta, delta);
    }

    /**
      * @brief Repeats a value within the range 0 -> _max.
      *
      * @param[in] _value The value to repeat within the provided range.
      * @param[in] _max The upper limit of the range.
      *
      * @return Returns the repeated value.
      */
    float repeat(in float _value, in float _max) {
		return mod(_max + mod(_value, _max), _max);
    }

     /**
      * @brief Repeats a value within the range _min -> _max.
      *
      * @param[in] _value The value to repeat within the provided range.
      * @param[in] _min The lower limit of the range.
      * @param[in] _max The upper limit of the range.
      *
      * @return Returns the repeated value.
      */
    float repeat(in float _value, in float _min, in float _max) {
		return _min + repeat(_value - _max, _max - _min);
    }

    /**
     * @brief Transforms a coordinate by applying the specified scale-translate parameter.
     *
     * @param[in] _uv The input vector to be transformed.
     * @param[in] _st The transformation parameters; a vec4 where .zw components are the translation and .xy are scaling factors.
     * @return vec2 Returns the transformed coordinates as a two-dimensional vector.
     */
    vec2 TransformCoord(in vec2 _uv, in vec4 _st) {
        return (_uv + fract(_st.zw)) * _st.xy;
    }

    /* TEXTURE SAMPLING */

    vec4 Sample4(in sampler2D _texture, in vec2 _uv) {
        return texture(_texture, _uv);
    }

    vec4 Sample4(in sampler2D _texture, in vec2 _uv, in int _lod) {
        return texture(_texture, _uv, _lod);
    }

    vec4 Sample4(in sampler2D _texture, in vec2 _uv, in vec4 _st) {
        return texture(_texture, TransformCoord(_uv, _st));
    }

    vec4 Sample4(in sampler2D _texture, in vec2 _uv, in vec4 _st, in int _lod) {
        return texture(_texture, TransformCoord(_uv, _st), _lod);
    }

    vec4 Sample4(in samplerCube _texture, in vec3 _dir) {
        return texture(_texture, _dir);
    }

    vec4 Sample4(in samplerCube _texture, in vec3 _dir, in int _lod) {
        return texture(_texture, _dir, _lod);
    }

    vec3 Sample3(in sampler2D _texture, in vec2 _uv) {
        return texture(_texture, _uv).rgb;
    }

    vec3 Sample3(in sampler2D _texture, in vec2 _uv, in int _lod) {
        return texture(_texture, _uv, _lod).rgb;
    }

    vec3 Sample3(in sampler2D _texture, in vec2 _uv, in vec4 _st) {
        return texture(_texture, TransformCoord(_uv, _st)).rgb;
    }

    vec3 Sample3(in sampler2D _texture, in vec2 _uv, in vec4 _st, in int _lod) {
        return texture(_texture, TransformCoord(_uv, _st), _lod).rgb;
    }

    vec3 Sample3(in samplerCube _texture, in vec3 _dir) {
        return texture(_texture, _dir).rgb;
    }

    vec3 Sample3(in samplerCube _texture, in vec3 _dir, in int _lod) {
        return texture(_texture, _dir, _lod).rgb;
    }

    vec2 Sample2(in sampler2D _texture, in vec2 _uv) {
        return texture(_texture, _uv).rg;
    }

    vec2 Sample2(in sampler2D _texture, in vec2 _uv, in int _lod) {
        return texture(_texture, _uv, _lod).rg;
    }

    vec2 Sample2(in sampler2D _texture, in vec2 _uv, in vec4 _st) {
        return texture(_texture, TransformCoord(_uv, _st)).rg;
    }

    vec2 Sample2(in sampler2D _texture, in vec2 _uv, in vec4 _st, in int _lod) {
        return texture(_texture, TransformCoord(_uv, _st), _lod).rg;
    }

    vec2 Sample2(in samplerCube _texture, in vec3 _dir) {
        return texture(_texture, _dir).rg;
    }

    vec2 Sample2(in samplerCube _texture, in vec3 _dir, in int _lod) {
        return texture(_texture, _dir, _lod).rg;
    }

    float Sample1(in sampler2D _texture, in vec2 _uv) {
        return texture(_texture, _uv).r;
    }

    float Sample1(in sampler2D _texture, in vec2 _uv, in int _lod) {
        return texture(_texture, _uv, _lod).r;
    }

    float Sample1(in sampler2D _texture, in vec2 _uv, in vec4 _st) {
        return texture(_texture, TransformCoord(_uv, _st)).r;
    }

    float Sample1(in sampler2D _texture, in vec2 _uv, in vec4 _st, in int _lod) {
        return texture(_texture, TransformCoord(_uv, _st), _lod).r;
    }

    float Sample1(in samplerCube _texture, in vec3 _dir) {
        return texture(_texture, _dir).r;
    }

    float Sample1(in samplerCube _texture, in vec3 _dir, in int _lod) {
        return texture(_texture, _dir, _lod).r;
    }

    vec3 SampleAmbient(in samplerCube _samplerTexture, in vec3 _dir, in float _blur) {

        float      s = textureSize(_samplerTexture, 0).x;
        float levels = log2(s);

        int b = int(pow(_blur, 2.0) * levels);

        // Sample the cubemap using the direction directly.
        return Sample3(_samplerTexture, _dir.zyx, b);
    }

    /**
     * @brief Calculates the luminance (brightness in an image) using one of three possible techniques.
     *
     * @param[in] _color The color for which to calculate the luminance.
     * @param[in] _technique The index of the technique to use: 0 - uses standard NTSC conversion weights,
     *                         1 - uses perceived brightness in color spaces, 2 - uses Pythagorean notion of brightness.
     *                         Any other values will return -1.0.
     *
     * @see Penov, F. and Slayden, G. (2016). image - Formula to determine perceived brightness of RGB color. [online] Stack Overflow. Available at: https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color [Accessed 15 Dec. 2023].
     *
     * @return float Luminance of the input color using the designated technique, or -1.0 if an invalid technique was passed.
     */
    float Luma(in vec3 _color, const int _technique) {

        float result;

        if (_technique == 0) {
            result = dot(_color, vec3(0.299, 0.587, 0.114));
        }
        else if (_technique == 1) {
            result = dot(_color, vec3(0.2126, 0.7152, 0.0722));
        }
        else if (_technique == 2) {
            result = sqrt(dot(dot(_color, _color), (0.299, 0.587, 0.114)));
        }
        else {
            result = -1.0;
        }

        return result;
    }

    /**
     * @brief Calculates the linear depth from the clipped space depth value.
     *
     * This function is used to linearize the depth information, converting it from a nonlinear [0,1]
     * range into a linear range on [near, far].
     *
     * @param[in] _depth Depth value in clipped space depth value. Typically, this value falls within the [0,1] range.
     * @param[in] _near Distance to the near clipping plane.
     * @param[in] _far Distance to the far clipping plane.
     *
     * @see JeGX (2009). How to Visualize the Depth Buffer in GLSL | Geeks3D. [online] Geeks3D. Available at: https://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/ [Accessed 15 Dec. 2023].
     *
     * @return float The linear depth value in the same unit as the input parameters (_near, _far).
     */
    float Linear01Depth(in float _depth, const float _near, const float _far) {
        return (2.0 * _near) / (_far + _near - _depth * (_far - _near));
    }

    /**
     * @brief Implements parallax mapping.
     *
     *
     * @param[in] _displacement The texture sampler for the displacement map.
     * @param[in] _viewDir The viewing direction vector.
     * @param[in] _uv The texture co-ordinates.
     * @param[in] _st The texture matrix for transformation.
     * @param[in] _scale The Scale for the parallax effect.
     *
     * @see de Vries, J. (n.d.). LearnOpenGL - Parallax Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Parallax-Mapping [Accessed 15 Dec. 2023].
     *
     * @return vec2 The final calculated texture co-ordinates after applying the parallax mapping.
     */
    #pragma optionNV(unroll all)
    vec2 ParallaxMapping(in sampler2D _displacement, in vec3 _viewDir, in vec2 _uv, in vec4 _st, in float _scale) {

        const float minLayers = 10.0;
        const float maxLayers = 32.0;
        float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), _viewDir), 0.0));

        // Calculate the size of each layer
        float layerDepth = 1.0 / numLayers;

        // Depth of current layer
        float currentLayerDepth = 0.0;

        // The amount to shift the texture coordinates per layer (from vector P)
        vec2 P = _viewDir.xy / _viewDir.z * (_scale / maxLayers);
        vec2 deltaTexCoords = P / numLayers;

        vec2  currentTexCoords     = _uv;
        float currentDepthMapValue = texture(_displacement, TransformCoord(_uv, _st)).r;

        while (currentLayerDepth < currentDepthMapValue) {
            // Shift texture coordinates along direction of P
            currentTexCoords -= deltaTexCoords;
            // Get depthmap value at current texture coordinates
            currentDepthMapValue = texture(_displacement, TransformCoord(currentTexCoords, _st)).r;
            // Get depth of next layer
            currentLayerDepth += layerDepth;
        }

        // Get texture coordinates before collision (reverse operations)
        vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

        // Get depth after and before collision for linear interpolation
        float afterDepth  = currentDepthMapValue - currentLayerDepth;
        float beforeDepth = texture(_displacement, TransformCoord(prevTexCoords, _st)).r - currentLayerDepth + layerDepth;

        // Interpolation of texture coordinates
        float weight = afterDepth / (afterDepth - beforeDepth);
        vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

        return finalTexCoords;
    }

#endif