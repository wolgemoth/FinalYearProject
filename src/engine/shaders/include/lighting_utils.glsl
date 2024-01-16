#ifndef _LIGHTING_UTILS
#define _LIGHTING_UTILS

    #include "/shaders/include/common_utils.glsl"
    #include "/shaders/include/rand.glsl"

    // Light attenuation function using the squared distance between the light and fragment.
    float Attenuation(in vec3 _lightPos, in vec3 _fragPos, in float _range) {
        return _range / length2(_lightPos, _fragPos);
    }

    // NVIDIA PCSS:
    //  - Fernando, R. and NVIDIA Corporation (2005). Percentage-Closer Soft Shadows. [online] Available at: https://developer.download.nvidia.com/shaderlibrary/docs/shadow_PCSS.pdf [Accessed 15 Dec. 2023].
    //  - Myers, K., Fernando , R., Bavoil, L. and NVIDIA Corporation (2008). Integrating Realistic Soft Shadows into Your Game Engine. [online] Available at: https://developer.download.nvidia.com/whitepapers/2008/PCSS_Integration.pdf [Accessed 15 Dec. 2023].

    // Hard and PCF shadows derived from implementations by Learn OpenGL:
    //  - de Vries, J. (n.d.). LearnOpenGL - Shadow Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping [Accessed 15 Dec. 2023].
    //  - de Vries, J. (n.d.). LearnOpenGL - Point Shadows. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows [Accessed 15 Dec. 2023].

    // Shadows for Parallax Occlusion Mapping. Implementation courtesy of "Rabbid76":
    // Rabbid76 (2019). Adding Shadows to Parallax Occlusion Map. [online] Stack Overflow. Available at: https://stackoverflow.com/questions/55089830/adding-shadows-to-parallax-occlusion-map [Accessed 15 Dec. 2023].
    float ParallaxShadowsHard(in sampler2D _displacement, in vec3 _lightDir, in vec2 _uv, in vec4 _st, in float _scale) {

        float minLayers = 0.0;
        float maxLayers = 16.0;
        float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), _lightDir)));

        vec2 currentTexCoords = _uv;
        float currentDepthMapValue = Sample1(_displacement, currentTexCoords, _st);
        float currentLayerDepth = currentDepthMapValue;

        float layerDepth = 1.0 / numLayers;
        vec2 P = _lightDir.xy / _lightDir.z * (_scale / maxLayers);
        vec2 deltaTexCoords = P / numLayers;

        while (currentLayerDepth <= currentDepthMapValue && currentLayerDepth > 0.0) {

            currentTexCoords += deltaTexCoords;
            currentDepthMapValue = Sample1(_displacement, currentTexCoords, _st);
            currentLayerDepth -= layerDepth;
        }

        return currentLayerDepth > currentDepthMapValue ? 1.0 : 0.0;
    }

    float PCSS_GetOccluderDepth3D(in samplerCube _shadowMap, in vec3 _dir, in float _texelSize, in float _bias, in float _seed, in int _samples) {

        float result = 0.0;

        int hits = 0;

        for (int i = 0; i < _samples; i++) {

            vec3 offset = normalize(Random3S(_dir + vec3(i + 1), _seed, 0.1));

            offset *= Random1S(_dir.xy + vec2(i + 1), _seed, 0.4);

            float occluderDepth = Sample1(_shadowMap, _dir + offset);

            if (occluderDepth < length(_dir) - _bias) {
                result += occluderDepth;

                hits++;
            }
        }

        if (hits == 0) {
            result = -1.0;
        }

        return result / float(max(hits, 1));
    }

    float PCSS_Radius3D(in samplerCube _shadowMap, in vec3 _dir, in float _texelSize, in float _bias, in float _lightSize, in float _seed, in int _samples) {

        float occluderDepth = PCSS_GetOccluderDepth3D(_shadowMap, _dir, _texelSize, _bias, _seed, _samples);

        if (occluderDepth == -1.0) {
            return 1.0;
        }

        float len = length(_dir);

        float penumbraWidth = (len - occluderDepth) / occluderDepth;

        return penumbraWidth * _lightSize;
    }

    float ShadowCalculationHard3D(in samplerCube _shadowMap, in vec3 _dir, in vec3 _offset, in float _bias, in float _lightRange) {

        // Get depth from light to closest surface.
        float shadowDepth = Sample1(_shadowMap, _dir + _offset) * _lightRange;

        // Compare the depth of the light against the distance from the light to the current fragment.
        return length(_dir) - _bias > shadowDepth ? 1.0 : 0.0;
    }

    float ShadowCalculationPCF3D(in samplerCube _shadowMap, in vec3 _dir, in float _texelSize, in float _bias, in float _radius, in float _lightRange) {

        float result = 0.0;

        float hits = 0.0;

        float PCF_SAMPLES = 8.0;

        float multiplier = 50.0; // Compensation for 3D.

        float axis = pow(PCF_SAMPLES, 1.0 / 3.0);
        for (float x = -_radius; x < _radius; x += _radius / axis) {
        for (float y = -_radius; y < _radius; y += _radius / axis) {
        for (float z = -_radius; z < _radius; z += _radius / axis) {

            vec3 offset = vec3(x, y, z);

            offset *= _texelSize * multiplier;

            result += ShadowCalculationHard3D(_shadowMap, _dir, offset, _bias, _lightRange);

            hits += 1.0;
        }}}

        return result / max(hits, 1.0);
    }

    float ShadowCalculationDisk3D(in samplerCube _shadowMap, in vec3 _dir, in float _texelSize, in float _bias, in float _radius, in float _lightRange, in float _seed, in int _samples) {

        float result = 0.0;

        float multiplier = 50.0; // Compensation for 3D.

        for (int i = 0; i < _samples; i++) {

            vec3 offset = normalize(Random3S(_dir + vec3(i + 1), fract(_seed), 0.1));

            offset *= _texelSize * _radius * multiplier * Random1U(_dir.xy + vec2(i + 1), fract(_seed), 0.4);

            result += ShadowCalculationHard3D(_shadowMap, _dir, offset, _bias, _lightRange);
        }

        return result / _samples;
    }

    float ShadowCalculationPCSS3D(in samplerCube _shadowMap, in vec3 _dir, in  float _texelSize, in float _bias, in float _lightRange,  in float _lightSize, in float _seed, in int _samples) {

        float result = 0.0;

        float radius = PCSS_Radius3D(_shadowMap, _dir, _texelSize, _bias, _lightSize, _seed, _samples);

        result = ShadowCalculationDisk3D(_shadowMap, _dir, _texelSize, _bias, radius, _lightRange, _seed, _samples);

        return result;
    }

    float PCSS_GetOccluderDepth2D(in sampler2D _shadowMap, in vec3 _fragPos, in float _texelSize, in float _bias, in float _seed, in int _samples) {

        float result = 0.0;

        int hits = 0;

        for (int i = 0; i < _samples; i++) {

            vec2 dir = Random2S(_fragPos.xy + vec2(i + 1), _seed, 0.1);

            dir *= float(i + 1.0) * _texelSize;

            float occluderDepth = Sample1(_shadowMap, _fragPos.xy + dir);

            if (occluderDepth < _fragPos.z - _bias) {
                result += occluderDepth;

                hits++;
            }
        }

        if (hits == 0) {
            result = -1;
        }

        return result / float(max(hits, 1));
    }

    float PCSS_Radius2D(in sampler2D _shadowMap, in vec3 _fragPos, in float _texelSize, in float _bias, in float _seed, in int _samples, in float _lightSize, in float _nearClip) {

        float occluderDepth = PCSS_GetOccluderDepth2D(_shadowMap, _fragPos, _texelSize, _bias, _seed, _samples);

        if (occluderDepth == -1.0) {
            return 1.0;
        }

        float penumbraWidth = (_fragPos.z - occluderDepth) / occluderDepth;

        return penumbraWidth * _lightSize * (_nearClip / _fragPos.z);
    }

    float ShadowCalculationHard2D(in sampler2D _shadowMap, in vec3 _fragPos, in vec2 _offset, in float _bias) {

        // Get depth from light to closest surface.
        float shadowDepth = Sample1(_shadowMap, _fragPos.xy + _offset);

        // Compare the depth of the light against the distance from the light to the current fragment (converted to light space).
        return (_fragPos.z <= 1.0) && (_fragPos.z - _bias > shadowDepth) ?
            1.0 : 0.0;
    }

    float ShadowCalculationPCF2D(in sampler2D _shadowMap, in vec3 _fragPos, in float _texelSize, in float _bias, in float _radius) {

        float result = 0.0;

        float hits = 0.0;

        float PCF_SAMPLES = 4.0;

        float axis = pow(PCF_SAMPLES, 1.0 / 2.0);
        for (float x = -_radius; x < _radius; x += _radius / axis) {
        for (float y = -_radius; y < _radius; y += _radius / axis) {

            vec2 dir = vec2(x, y);

            dir *= _texelSize;

            result += ShadowCalculationHard2D(_shadowMap, _fragPos, dir, _bias);

            hits += 1.0;
        }}

        return result / max(hits, 1.0);
    }

    float ShadowCalculationDisk2D(in sampler2D _shadowMap, in vec3 _fragPos, in float _texelSize, in float _bias, in float _radius, in float _seed, in int _samples) {

        float result = 0.0;

        for (int i = 0; i < _samples; i++) {

            vec2 dir = normalize(Random2S(_fragPos.xy + vec2(i + 1), _seed, 0.1));

            dir *= _texelSize * _radius * (float(i + 1) / float(_samples));

            result += ShadowCalculationHard2D(_shadowMap, _fragPos, dir, _bias);
        }

        return result / _samples;
    }

    float ShadowCalculationPCSS2D(in sampler2D _shadowMap, in vec3 _fragPos, in float _texelSize, in float _bias, in float _nearClip, in float _lightAngle, in float _lightSize, in float _seed, in int _samples) {

        float result = 0.0;

        float perspective_multiplier = _lightAngle == -1.0 ? 1.0 : 32.0;

        float radius = PCSS_Radius2D(_shadowMap, _fragPos, _texelSize, _bias, _seed, _samples, _lightSize, _nearClip) / _texelSize;

        result = ShadowCalculationDisk2D(_shadowMap, _fragPos, _texelSize, _bias, radius * perspective_multiplier, _seed, _samples);

        return result;
    }

#endif