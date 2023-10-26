#version 330

in vec3 v_TexCoord;

uniform samplerCube u_Texture;

void main() {
    gl_FragColor = texture(u_Texture, v_TexCoord);
}