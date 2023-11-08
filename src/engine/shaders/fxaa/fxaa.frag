#version 330 core

#extension GL_ARB_explicit_uniform_location : enable

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

// Trims the algorithm from processing darks.
//   0.0833 - upper limit (default, the start of visible unfiltered edges)
//   0.0625 - high quality (faster)
//   0.0312 - visible limit (slower)
uniform float u_ContrastThreshold;

// The minimum amount of local contrast required to apply algorithm.
//   0.333 - too little (faster)
//   0.250 - low quality
//   0.166 - default
//   0.125 - high quality
//   0.063 - overkill (slower)
uniform float u_RelativeThreshold;

// Choose the amount of sub-pixel aliasing removal.
// This can effect sharpness.
//   1.00 - upper limit (softer)
//   0.75 - default amount of filtering
//   0.50 - lower limit (sharper, less sub-pixel aliasing removal)
//   0.25 - almost off
//   0.00 - completely off
uniform float u_SubpixelBlending;

// Uncomment ENHANCED_FXAA to enable various FXAA enhancements designed
// by myself (Loui Eriksson). These will moderately impact performance
// in exchange for higher quality antialiasing.
#define ENHANCED_FXAA

// If ENHANCED_FXAA is enabled, increase the size of the luma kernel.
// A wider kernel results in thicker edges, which can help resolve
// artifacts when sampling along the edge.
#ifdef ENHANCED_FXAA
    const int KERNEL_SIZE = 5;
#else
    const int KERNEL_SIZE = 3;
#endif

// A very large number without overflow.
const float INFINITY = 65535.0;

struct Luminance {

    float samples[KERNEL_SIZE * KERNEL_SIZE];

    float maximal;
    float minimal;
    float average;

    vec2 direction;

    float contrast;
};

float sqr(in float _value) {
    return _value * _value;
}

float Luma(vec3 _color){

    // https://stackoverflow.com/questions/596216/formula-to-determine-perceived-brightness-of-rgb-color

    //return dot(_color, vec3(0.2126, 0.7152, 0.0722));
    return dot(_color, vec3(0.299, 0.587, 0.114));
    //return sqrt(dot(dot(_color, _color), (0.299, 0.587, 0.114)));
}

// https://catlikecoding.com/unity/tutorials/advanced-rendering/fxaa/
Luminance SampleLuma(in vec2 _coord, in vec2 _texelSize) {

    Luminance result;
    result.maximal = -INFINITY;
    result.minimal =  INFINITY;
    result.average = 0;
    result.direction = vec2(0);

    for (int i = 0; i < sqr(KERNEL_SIZE); i++) {
        result.samples[i] = 0;
    }

    int w = ((KERNEL_SIZE + 1) / 2) - 1;

    int s = 0;

    for (int i = -w; i <= w; i++) {
    for (int j = -w; j <= w; j++) {

        s++;

        vec2 offset = vec2(i, j);

        result.samples[s] = Luma(texture(u_Texture, _coord + (offset * _texelSize), 0).rgb);

        result.maximal = max(result.maximal, result.samples[s]);
        result.minimal = min(result.minimal, result.samples[s]);

        // Distance from middle to sample.
        float f = float(max(abs(i) + abs(j), 1)); //max(length(offset), 1.0);//

        result.average += result.samples[s] / f;

        result.direction += offset * (result.samples[s] / f);
    }}

    result.contrast = result.maximal - result.minimal;

    result.average   /= sqr(KERNEL_SIZE);
    result.direction /= sqr(KERNEL_SIZE);

    return result;
}

vec3 FXAA(in Luminance _luma, in vec2 _texelSize) {

    int center = (((KERNEL_SIZE * KERNEL_SIZE) + 1) / 2) - 1;

    // Filter to selectively blend pixels based on their contrast.
    // Without filtering, all 'edges' recieve smoothing equally,
    // which contributes to an overall softening of the image.
    float filter = abs(_luma.average - _luma.samples[center]);
    filter = min(filter / _luma.contrast, 1.0);

    // Morph the filter from a linear curve to a quintic one,
    // for a much smoother transition between sharp and smooth details.
    float blend = sqr(smoothstep(1.0, 0.0, filter));

    vec2 dir = _luma.direction;

    // If ENHANCED_FXAA is disabled, flatten the direction to
    // only X or Y. This results in cross-shaped edge blending.
    #ifndef ENHANCED_FXAA
        dir.x *= step(abs(dir.y), abs(dir.x));
        dir.y *= step(abs(dir.x), abs(dir.y));
    #endif

    dir = normalize(dir) * blend * u_SubpixelBlending;

    // If ENHANCED_FXAA is enabled, blend along both sides of
    // the edge instead of blending the center pixel with a
    // neighbor. Blending across both sides of the edge helps
    // correct 'haloing' caused by propagation of bright
    // samples when using FXAA with HDR.
    #ifdef ENHANCED_FXAA

        // Since we're sampling both sides of the edge, the direction needs to be halved.
        dir /= 2.0;

        vec3 s1 = texture(u_Texture, v_TexCoord - (dir * _texelSize), 0).rgb;
    #else
        vec3 s1 = texture(u_Texture, v_TexCoord, 0).rgb;
    #endif

    vec3 s2 = texture(u_Texture, v_TexCoord + (dir * _texelSize), 0).rgb;

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
        color = texture(u_Texture, v_TexCoord, 0).rgb;//vec3(0);//
    }

    //color -= texture(u_Texture, v_TexCoord, 0).rgb;

    //color = min(abs(color - texture(u_Texture, v_TexCoord, 0).rgb) * INFINITY, vec3(1.0)).rrr;

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