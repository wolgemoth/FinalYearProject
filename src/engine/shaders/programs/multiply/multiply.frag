#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;

/* PARAMETERS */
uniform float u_Strength;

void main() {
	gl_FragColor = texture2D(u_Texture0, v_TexCoord) * (texture2D(u_Texture1, v_TexCoord) * u_Strength);
}