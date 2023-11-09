#version 330 core

#extension GL_ARB_explicit_uniform_location : enable

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;

in vec2 v_TexCoord;

/* PARAMETERS */
layout (location = 2) uniform float u_Strength;

void main() {
	gl_FragColor = texture2D(u_Texture0, v_TexCoord) - (texture2D(u_Texture1, v_TexCoord) * u_Strength);
}