#version 330 core

uniform sampler2D u_Texture;

in vec2 v_TexCoord;

/* PARAMETERS */
uniform vec2 u_Resolution;
uniform float u_Diffusion;

vec4 BoxBlur(vec2 _coordinate, float _diffusion) {

	vec2 l = vec2(1.0) / u_Resolution;
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
	gl_FragColor = BoxBlur(v_TexCoord, u_Diffusion);
}