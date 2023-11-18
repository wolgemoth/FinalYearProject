#pragma vertex

    #version 330 core

    layout (location = 0) in vec2 a_Position;
    layout (location = 1) in vec2 a_TexCoord;

    out vec2 v_TexCoord;

    void main() {

      v_TexCoord = a_TexCoord;

      gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_explicit_uniform_location : enable

    const float INFINITY = 65535.0; // A 'sufficiently' large number.
    const float  EPSILON = 0.005;   // A 'sufficiently' small number.

    in vec2 v_TexCoord;

    uniform sampler2D u_Texture;

    // Trims the algorithm from processing darks.
    //   0.0833 - upper limit (default, the start of visible unfiltered edges)
    //   0.0625 - high quality (faster)
    //   0.0312 - visible limit (slower)
    uniform float u_ContrastThreshold = 0.0312;

    // The minimum amount of local contrast required to apply algorithm.
    //   0.333 - too little (faster)
    //   0.250 - low quality
    //   0.166 - default
    //   0.125 - high quality
    //   0.063 - overkill (slower)
    uniform float u_RelativeThreshold = 0.063;

    // Choose the amount of sub-pixel aliasing removal.
    // This can effect sharpness.
    //   1.00 - upper limit (softer)
    //   0.75 - default amount of filtering
    //   0.50 - lower limit (sharper, less sub-pixel aliasing removal)
    //   0.25 - almost off
    //   0.00 - completely off
    uniform float u_SubpixelBlending = 1.0;

    // Choose the amount of edge aliasing removal.
    // This can effect sharpness.
    //   1.00 - upper limit (softer)
    //   0.75 - default amount of filtering
    //   0.50 - lower limit (sharper, less edge-aliasing removal)
    //   0.25 - almost off
    //   0.00 - completely off
    uniform float u_EdgeBlending = 1.0;

    // Higher numbers typically lead to reduced softening when combined with subpixel blending.
    uniform float u_LocalContrastModifier = 1.0;

    // Uncomment ENHANCED_FXAA to enable various FXAA enhancements designed
    // by myself (Loui Eriksson). These will moderately impact performance
    // in exchange for higher quality antialiasing.
    #define ENHANCED_FXAA

    const int KERNEL_SIZE = 3;

    struct Luminance {

        float samples[KERNEL_SIZE * KERNEL_SIZE];

        float maximal;
        float minimal;
        float average;

        vec2 direction;

        float contrast;
    };

    int sqr(in int _value) {
        return _value * _value;
    }

    float sqr(in float _value) {
        return _value * _value;
    }

    float Luma(vec3 _color) {

        // https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color

        //return dot(_color, vec3(0.2126, 0.7152, 0.0722));
        return dot(_color, vec3(0.299, 0.587, 0.114));
        //return sqrt(dot(dot(_color, _color), (0.299, 0.587, 0.114)));
    }

    // https://catlikecoding.com/unity/tutorials/advanced-rendering/fxaa/
    Luminance SampleLuma(in vec2 _coord, in vec2 _texelSize) {

        Luminance result;
        result.maximal   = -INFINITY;
        result.minimal   =  INFINITY;
        result.average   = 0;
        result.direction = vec2(0);

        for (int i = 0; i < sqr(KERNEL_SIZE); i++) {
            result.samples[i] = 0;
        }

        int w = ((KERNEL_SIZE + 1) / 2) - 1;

        int s = 0;

        for (int i = -w; i <= w; i++) {
        for (int j = -w; j <= w; j++) {

            vec2 offset = vec2(i, j);

            vec2 uv = clamp(_coord + (offset * _texelSize), vec2(0), vec2(1));

            float luma          = Luma(texture(u_Texture, uv).rgb);
            float luma_adjusted = luma / max(abs(i) + abs(j), 1);

            result.maximal = max(result.maximal, luma);
            result.minimal = min(result.minimal, luma);

            result.average   +=          luma_adjusted;
            result.direction += offset * luma_adjusted;

            result.samples[s] = luma;

            s++;
        }}

        result.contrast = result.maximal - result.minimal;

        result.average   /= sqr(KERNEL_SIZE);
        result.direction /= sqr(KERNEL_SIZE);

        return result;
    }

    int To1D(ivec2 _index, int _blockWidth) {
        return (_index.y * _blockWidth) + _index.x;
    }

    float DetermineEdgeBlendFactor(in Luminance _luma, in vec2 _texelSize) {

        int EDGE_SEARCHES = 10;

        int center = (((KERNEL_SIZE * KERNEL_SIZE) + 1) / 2) - 1;

        int w = ((KERNEL_SIZE + 1) / 2) - 1;

        vec2 edgeDir = _luma.direction.yx;

        float gradient = max(abs(_luma.direction.x), abs(_luma.direction.y));

        float edgeLuminance = (
            _luma.samples[center] +
            _luma.samples[To1D(ivec2(normalize(-edgeDir)) + ivec2(w), KERNEL_SIZE)]
        ) * 0.5;

        float gradientThreshold = gradient * 0.25;

        vec2 edgeStep = (edgeDir * _texelSize * 0.5);

        vec2 puv = v_TexCoord;
        float pLuminanceDelta = 0.0;
        bool pAtEnd = false;

        for (int i = 0; i < EDGE_SEARCHES && !pAtEnd; i++) {
            puv += edgeStep;
            pLuminanceDelta = Luma(texture(u_Texture, puv * _texelSize).rgb) - edgeLuminance;
            pAtEnd = abs(pLuminanceDelta) >= gradientThreshold;
        }

        vec2 nuv = v_TexCoord;
        float nLuminanceDelta = 0.0;
        bool nAtEnd = false;

        for (int i = 0; i < EDGE_SEARCHES && !nAtEnd; i++) {
            nuv -= edgeStep;
            nLuminanceDelta = Luma(texture(u_Texture, nuv * _texelSize).rgb) - edgeLuminance;
            nAtEnd = abs(nLuminanceDelta) >= gradientThreshold;
        }

        vec2 distances = vec2(
            abs(length(puv - v_TexCoord)),
            abs(length(nuv - v_TexCoord))
        );

        bool deltaSign;
        if (distances.x <= distances.y) {
            deltaSign = pLuminanceDelta >= 0;
        }
        else {
            deltaSign = nLuminanceDelta >= 0;
        }

        if (deltaSign == (_luma.samples[center] - edgeLuminance >= 0)) {
            return 0.0;
        }

        return 1.0 - clamp(
            ((min(distances.x, distances.y) / max(_texelSize.x, _texelSize.y)) * float(EDGE_SEARCHES)),
            0.0,
            1.0
        );
    }

    vec3 FXAA(in Luminance _luma, in vec2 _texelSize) {

        int center = (((KERNEL_SIZE * KERNEL_SIZE) + 1) / 2) - 1;

        _luma.contrast = _luma.contrast / max(u_LocalContrastModifier, EPSILON);

        // If ENHANCED_FXAA is disabled, flatten the direction to
        // only X or Y. This results in cross-shaped edge blending.
        #ifndef ENHANCED_FXAA
            _luma.direction.x *= step(abs(_luma.direction.y), abs(_luma.direction.x));
            _luma.direction.y *= step(abs(_luma.direction.x), abs(_luma.direction.y));
        #endif

        // Filter to selectively blend pixels based on their local contrast.
        // Without filtering, all 'edges' recieve smoothing equally.
        float localContrastFilter = abs(_luma.average - _luma.samples[center]);
        localContrastFilter = sqr(smoothstep(0.0, 1.0, 1.0 - min(localContrastFilter / _luma.contrast, 1.0)));

        float finalBlend = max(

            /* SUBPIXEL BLENDING */
            localContrastFilter * u_SubpixelBlending,

            /* EDGE BLENDING */
            DetermineEdgeBlendFactor(_luma, _texelSize) * u_EdgeBlending
        );

        vec2 dir = normalize(_luma.direction) * finalBlend;

        // If ENHANCED_FXAA is enabled, blend along both sides of
        // the edge instead of blending the center pixel with a
        // neighbor. Blending across both sides of the edge yields
        // a softer result and can help mitigate the propagation of
        // HDR values throughout the image.
        #ifdef ENHANCED_FXAA

            // Since we're sampling both sides of the edge, the direction needs to be halved.
            dir /= 2.0;

            vec3 s1 = texture(u_Texture, v_TexCoord - (dir * _texelSize)).rgb;
        #else
            vec3 s1 = texture(u_Texture, v_TexCoord).rgb;
        #endif

        vec3 s2 = texture(u_Texture, v_TexCoord + (dir * _texelSize)).rgb;

        // Return the average of the two samples.
        return (s1 + s2) * 0.5;
    }

    void main() {

        vec2 texelSize = vec2(1.0) / textureSize(u_Texture, 0);

        Luminance luma = SampleLuma(v_TexCoord, texelSize);

        float threshold = max(u_ContrastThreshold, u_RelativeThreshold * luma.maximal);

        vec3 color;

        if (luma.contrast > threshold) {
            color = vec3(FXAA(luma, texelSize));
        }
        else {
            color = texture(u_Texture, v_TexCoord).rgb;//vec3(0);//
        }

        //color -= texture(u_Texture, v_TexCoord).rgb;

        //color = min(abs(color - texture(u_Texture, v_TexCoord).rgb) * INFINITY, vec3(1.0)).rrr;

    //    if (abs(luma.direction.x) > abs(luma.direction.y)) {
    //
    //        if (luma.direction.x > 0) {
    //            color *= vec3(1, 0, 0);
    //        }
    //        else {
    //            color *= vec3(0, 0, 1);
    //        }
    //    }
    //    else {
    //        if (luma.direction.y > 0) {
    //            color *= vec3(0, 1, 0);
    //        }
    //        else {
    //            color *= vec3(1, 0, 1);
    //        }
    //    }

        gl_FragColor = vec4(color, 1.0);
    }