#version 330 core

#extension GL_ARB_explicit_uniform_location : enable

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() {

    // Get the depth value of the current fragment
    vec3 color = texture(u_Texture, v_TexCoord).rgb;

    color = vec3(1, 0, 1);

	gl_FragColor = vec4(color, 1.0);
}