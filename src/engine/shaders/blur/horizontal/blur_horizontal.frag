#version 330 core

#extension GL_ARB_explicit_uniform_location : enable

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

uniform float u_Strength = 1.0;

uniform float u_Step = 2.0;

vec4 BlurHorizontal(vec2 _coordinate, float _diffusion) {

	vec2 l = (vec2(1.0) / textureSize(u_Texture, 0)) * _diffusion;

	vec4 s =
		texture2D(u_Texture, v_TexCoord + vec2(-l.x, 0)) +
		texture2D(u_Texture, v_TexCoord                ) +
		texture2D(u_Texture, v_TexCoord + vec2( l.x, 0));

	// Average.
	return s / 3.0;
}

void main() {

    vec3 color = BlurHorizontal(v_TexCoord, u_Step).rgb;

	gl_FragColor = vec4(color, 1.0);
}