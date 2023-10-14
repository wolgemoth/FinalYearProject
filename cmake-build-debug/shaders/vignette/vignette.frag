#version 330 core

#extension GL_ARB_explicit_uniform_location : enable

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

/* PARAMETERS */
uniform float u_Amount;

void main() {

    vec2 uv = (v_TexCoord - 0.5) * 2.0;

	gl_FragColor = vec4(texture2D(u_Texture, v_TexCoord)) * (1.0 - distance(uv * u_Amount, vec2(0, 0)));
}
