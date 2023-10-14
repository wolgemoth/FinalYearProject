#version 330 core

#extension GL_ARB_explicit_uniform_location : enable

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

/* PARAMETERS */
uniform float u_Amount;
uniform float u_Time;

// srand3, terrible pseudorandom number generator.
float Rand(vec2 _uv) {
    return fract(sin(dot(_uv, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {

	float grain = Rand((v_TexCoord + u_Time) * 0.1) * u_Amount;

	gl_FragColor = vec4(texture2D(u_Texture, v_TexCoord)) - grain;
}