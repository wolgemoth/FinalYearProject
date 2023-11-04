#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

uniform float u_NearClip;
uniform float u_FarClip;

// https://www.geeks3d.com/20091216/geexlab-how-to-visualize-the-depth-buffer-in-glsl/
float Linear01Depth(float _depth) {
    return (2.0 * u_NearClip) / (u_FarClip + u_NearClip - _depth * (u_FarClip - u_NearClip));
}

void main() {

    // Get the depth value of the current fragment
    float depth = 1.0 - Linear01Depth(texture(u_Texture, v_TexCoord).r);

	gl_FragColor = vec4(depth, depth, depth, 1.0);
}