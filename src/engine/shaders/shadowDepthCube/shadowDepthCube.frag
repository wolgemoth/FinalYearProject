#version 330 core

in vec4 FragPos;

uniform vec3 u_LightPosition;
uniform float u_FarPlane;

void main() {

    // Get distance between fragment and light source.
    float lightDistance = length(FragPos.xyz - u_LightPosition);

    // Map to [0;1] range by dividing by far_plane.
    lightDistance = lightDistance / u_FarPlane;

    // Write this as modified depth.
    gl_FragDepth = lightDistance;
}