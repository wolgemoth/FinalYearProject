#version 330 core

#extension GL_ARB_explicit_uniform_location : enable

in vec2 v_TexCoord;

uniform sampler2D u_Color;
uniform sampler2D u_Depth;

uniform float u_NearClip;
uniform float u_FarClip;

uniform float u_Time;

float PHI = 1.61803398874989484820459;  // Φ = Golden Ratio

float gold_noise(in vec2 xy, in float seed){
    return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

float Random1(in vec2 _xy, float _offset) {

    return (gold_noise(_xy * 1000.0, fract(u_Time) + _offset) - 0.5) * 2.0;
}

vec2 Random2(in vec2 _xy, float _offset) {

    return vec2(
        Random1(_xy, _offset + 0.1),
        Random1(_xy, _offset + 0.2)
    );
}

vec3 Random3(in vec3 _xyz, float _offset) {

    return vec3(
        Random1(_xyz.xy, _offset + 0.1),
        Random1(_xyz.yz, _offset + 0.2),
        Random1(_xyz.xz, _offset + 0.3)
    );
}

// https://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/
float Linear01Depth(float _depth) {
    return (2.0 * u_NearClip) / (u_FarClip + u_NearClip - _depth * (u_FarClip - u_NearClip));
}

void main() {

    // Get the depth value of the current fragment
    float depth = Linear01Depth(texture(u_Depth, v_TexCoord).r) * u_FarClip;

    float occlusion = 0.0;

    int samples = 32;

    float radiusXY = 0.05;
    float radiusZ = 2;

    float bias = -0.5;
    float strength = 0.8;

    for (int i = 0; i < samples; i++) {

        vec2 offset = normalize(Random2(v_TexCoord + vec2(i + 1), 0.1));

        offset *= radiusXY * Random1(v_TexCoord + vec2(i + 1), 0.3);

        float sampleDepth = Linear01Depth(texture(u_Depth, v_TexCoord + offset.xy).r) * u_FarClip;

        float rangeCheck = smoothstep(0.0, 1.0, radiusZ / abs(depth - sampleDepth));

        //occlusion += rangeCheck;
        //occlusion += (sampleDepth >= depth + bias ? 0.0 : 1.0);

        occlusion += (sampleDepth >= depth + bias ? 0.0 : 1.0) * rangeCheck;
    }

    occlusion = 1.0 - ((occlusion / float(samples)) * strength);

    vec4 color = texture(u_Color, v_TexCoord);

    // color *

	gl_FragColor = color * vec4(occlusion, occlusion, occlusion, 1.0);
}