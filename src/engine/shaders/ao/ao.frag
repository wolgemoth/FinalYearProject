#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() {

    // Get the depth value of the current fragment
    float depth = texture(u_Texture, v_TexCoord).r;

	gl_FragColor = vec4(depth, depth, depth, 1.0);
}