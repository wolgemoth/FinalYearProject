#version 330 core

#extension GL_ARB_explicit_uniform_location : enable

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

uniform int u_Samples = 32;

uniform float u_Strength = 1.0;

vec4 BoxBlur(vec2 _coordinate, float _diffusion) {

	vec2 l = vec2(1.0) / textureSize(u_Texture, 0);
	l -= l / 2.0;
	l *= 2.0 * _diffusion;

	// Get samples.
	vec4 s  = texture2D(u_Texture, v_TexCoord + vec2(-l.x, -l.y));
	     s += texture2D(u_Texture, v_TexCoord + vec2( l.x, -l.y));
	     s += texture2D(u_Texture, v_TexCoord + vec2(-l.x,  l.y));
	     s += texture2D(u_Texture, v_TexCoord + vec2( l.x,  l.y));

	// Average.
	return s / 4.0;
}

void main() {

    vec3 color = BoxBlur(v_TexCoord, 2.0).rgb;

	gl_FragColor = vec4(color, 1.0);
}