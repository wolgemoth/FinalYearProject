#version 330 core

uniform sampler2D u_Texture;

in vec2 v_TexCoord;

/* PARAMETERS */
uniform float u_Threshold;
uniform float u_Clamp;

void main() {
	gl_FragColor = min(max(texture2D(u_Texture, v_TexCoord) - u_Threshold, 0.0), vec4(u_Clamp));
}