#version 330

in vec3 v_TexCoord;

uniform samplerCube u_Texture;

uniform float u_Exposure = 1.0;

void main() {
    gl_FragColor = texture(u_Texture, v_TexCoord) * u_Exposure;
}