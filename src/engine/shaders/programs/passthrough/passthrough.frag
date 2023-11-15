#version 330 core

uniform sampler2D u_Texture;

in vec2 v_TexCoord;

void main() {
	gl_FragColor = texture2D(u_Texture, v_TexCoord);
}
