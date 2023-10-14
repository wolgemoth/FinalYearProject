#version 330 core

uniform sampler2D u_Texture;

in vec2 v_TexCoord;

/* PARAMETERS */
uniform float u_Threshold;

void main() {
	gl_FragColor = max(texture2D(u_Texture, v_TexCoord) - u_Threshold, 0.0);
}
