#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_Matrices[6];

out vec4 FragPos; // FragPos from geometry shader. (Output per EmitVertex())

void main() {

    for (int face = 0; face < 6; ++face) {

        // Built-in variable that specifies to which face we render.
        gl_Layer = face;

        // For each triangle vertex...
        for (int i = 0; i < 3; ++i) {

            FragPos = gl_in[i].gl_Position;

            gl_Position = u_Matrices[face] * FragPos;

            EmitVertex();
        }

        EndPrimitive();
    }
}