#pragma vertex

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    layout (location = 0) in mediump vec2 a_Position;
    layout (location = 1) in mediump vec2 a_TexCoord;

    out mediump vec2 v_TexCoord;

    void main() {

        v_TexCoord = a_TexCoord;

        gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    #include "/assets/shaders/core/include/constants.inc"
    #include "/assets/shaders/core/include/common_utils.inc"

    in mediump vec2 v_TexCoord;

    uniform sampler2D u_Texture;

    // Trims the algorithm from processing darks.
    //   0.0833 - upper limit (default, the start of visible unfiltered edges)
    //   0.0625 - high quality (faster)
    //   0.0312 - visible limit (slower)
    uniform mediump float u_ContrastThreshold = 0.0312;

    // The minimum amount of local contrast required to apply algorithm.
    //   0.333 - too little (faster)
    //   0.250 - low quality
    //   0.166 - default
    //   0.125 - high quality
    //   0.063 - overkill (slower)
    uniform mediump float u_RelativeThreshold = 0.063;

    // Choose the amount of sub-pixel aliasing removal.
    // This can effect sharpness.
    //   1.00 - upper limit (softer)
    //   0.75 - default amount of filtering
    //   0.50 - lower limit (sharper, less sub-pixel aliasing removal)
    //   0.25 - almost off
    //   0.00 - completely off
    uniform mediump float u_SubpixelBlending = 1.0;

    // Choose the amount of edge aliasing removal.
    // This can effect sharpness.
    //   1.00 - upper limit (softer)
    //   0.75 - default amount of filtering
    //   0.50 - lower limit (sharper, less edge-aliasing removal)
    //   0.25 - almost off
    //   0.00 - completely off
    uniform mediump float u_EdgeBlending = 1.0;

    // Higher numbers typically lead to reduced softening when combined with subpixel blending.
    uniform mediump float u_LocalContrastModifier = 1.0;

    // Uncomment ENHANCED_FXAA to enable various FXAA enhancements designed
    // by myself (Loui Eriksson). These will moderately impact performance
    // in exchange for higher quality antialiasing.
    #define ENHANCED_FXAA

    const int KERNEL_SIZE = 3;

    struct Luminance {

        mediump float samples[KERNEL_SIZE * KERNEL_SIZE];

        mediump float maximal;
        mediump float minimal;
        mediump float average;

        mediump vec2 direction;

        mediump float contrast;
    };

    // https://catlikecoding.com/unity/tutorials/advanced-rendering/fxaa/
    Luminance SampleLuma(in mediump vec2 _coord, in mediump vec2 _texelSize) {

        Luminance result;
        result.maximal   = F32NMAX;
        result.minimal   = F32PMAX;
        result.average   = 0;
        result.direction = vec2(0);

        for (int i = 0; i < sqr(KERNEL_SIZE); ++i) {
            result.samples[i] = 0;
        }

        int w = ((KERNEL_SIZE + 1) / 2) - 1;

        int s = 0;

        for (int i = -w; i <= w; ++i) {
        for (int j = -w; j <= w; j++) {

            mediump vec2 offset = vec2(i, j);

            mediump vec2 uv = clamp(_coord + (offset * _texelSize), vec2(0.0), vec2(1.0));

            mediump float luma          = Luma(Sample3(u_Texture, uv), 0);
            mediump float luma_adjusted = luma / float(max(abs(i) + abs(j), 1));

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

    int To1D(in ivec2 _index, in int _blockWidth) {
        return (_index.y * _blockWidth) + _index.x;
    }

    float DetermineEdgeBlendFactor(in Luminance _luma, in mediump vec2 _texelSize) {

        int EDGE_SEARCHES = 10;

        int center = (((KERNEL_SIZE * KERNEL_SIZE) + 1) / 2) - 1;

        int w = ((KERNEL_SIZE + 1) / 2) - 1;

        mediump vec2 edgeDir = _luma.direction.yx;

        mediump float gradient = max(abs(_luma.direction.x), abs(_luma.direction.y));

        mediump float edgeLuminance = (
            _luma.samples[center] +
            _luma.samples[To1D(ivec2(normalize(-edgeDir)) + ivec2(w), KERNEL_SIZE)]
        ) * 0.5;

        mediump float gradientThreshold = gradient * 0.25;

        mediump vec2 edgeStep = (edgeDir * _texelSize * 0.5);

        mediump vec2 puv = v_TexCoord;
        mediump float pLuminanceDelta = 0.0;
        bool pAtEnd = false;

        for (int i = 0; i < EDGE_SEARCHES && !pAtEnd; ++i) {
            puv += edgeStep;
            pLuminanceDelta = Luma(Sample3(u_Texture, puv * _texelSize), 0) - edgeLuminance;
            pAtEnd = abs(pLuminanceDelta) >= gradientThreshold;
        }

        mediump vec2 nuv = v_TexCoord;
        mediump float nLuminanceDelta = 0.0;
        bool nAtEnd = false;

        for (int i = 0; i < EDGE_SEARCHES && !nAtEnd; ++i) {
            nuv -= edgeStep;
            nLuminanceDelta = Luma(Sample3(u_Texture, nuv * _texelSize), 0) - edgeLuminance;
            nAtEnd = abs(nLuminanceDelta) >= gradientThreshold;
        }

        mediump vec2 distances = vec2(
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

    mediump vec3 FXAA(in Luminance _luma, in mediump vec2 _texelSize) {

        int center = (((KERNEL_SIZE * KERNEL_SIZE) + 1) / 2) - 1;

        _luma.contrast = _luma.contrast / max(u_LocalContrastModifier, KEPSILON);

        // If ENHANCED_FXAA is disabled, flatten the direction to
        // only X or Y. This results in cross-shaped edge blending.
        #ifndef ENHANCED_FXAA
            _luma.direction.x *= step(abs(_luma.direction.y), abs(_luma.direction.x));
            _luma.direction.y *= step(abs(_luma.direction.x), abs(_luma.direction.y));
        #endif

        // Filter to selectively blend pixels based on their local contrast.
        // Without filtering, all 'edges' recieve smoothing equally.
        mediump float localContrastFilter = abs(_luma.average - _luma.samples[center]);
        localContrastFilter = sqr(smoothstep(0.0, 1.0, 1.0 - min(localContrastFilter / _luma.contrast, 1.0)));

        mediump float finalBlend = max(

            /* SUBPIXEL BLENDING */
            localContrastFilter * u_SubpixelBlending,

            /* EDGE BLENDING */
            DetermineEdgeBlendFactor(_luma, _texelSize) * u_EdgeBlending
        );

        mediump vec2 dir = normalize(_luma.direction) * finalBlend;

        // If ENHANCED_FXAA is enabled, blend along both sides of
        // the edge instead of blending the center pixel with a
        // neighbor. Blending across both sides of the edge yields
        // a softer result and can help mitigate the propagation of
        // HDR values throughout the image.
        #ifdef ENHANCED_FXAA

            // Since we're sampling both sides of the edge, the direction needs to be halved.
            dir /= 2.0;

            mediump vec3 s1 = Sample3(u_Texture, v_TexCoord - (dir * _texelSize));
        #else
            mediump vec3 s1 = Sample3(u_Texture, v_TexCoord).rgb;
        #endif

        mediump vec3 s2 = Sample3(u_Texture, v_TexCoord + (dir * _texelSize));

        // Return the average of the two samples.
        return (s1 + s2) * 0.5;
    }

    void main() {

        // This shader is heavily derived from an FXAA implementation provided by "Catlike Coding".
        // Flick, J. (2018). FXAA. [online] catlikecoding.com. Available at: https://catlikecoding.com/unity/tutorials/advanced-rendering/fxaa/ [Accessed 15 Dec. 2023].

        mediump vec2 texelSize = vec2(1.0) / textureSize(u_Texture, 0);

        Luminance luma = SampleLuma(v_TexCoord, texelSize);

        mediump float threshold = max(u_ContrastThreshold, u_RelativeThreshold * luma.maximal);

        mediump vec3 color;

        if (luma.contrast > threshold) {
            color = vec3(FXAA(luma, texelSize));
        }
        else {
            color = Sample3(u_Texture, v_TexCoord).rgb;
        }

        gl_FragColor = vec4(color, 1.0);
    }