#version 330 core

in vec2 a_Position;
in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main() {

  v_TexCoord = a_TexCoord;

  gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
}
