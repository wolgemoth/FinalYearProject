#version 330 core

in vec4 FragPos;

uniform vec3 u_LightPosition;
uniform float u_FarPlane;

void main() {
    gl_FragDepth = length(FragPos.xyz - u_LightPosition) / u_FarPlane;
}