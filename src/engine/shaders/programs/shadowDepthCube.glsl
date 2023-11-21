#pragma vertex

    #version 330 core

    layout (location = 0) in vec3 a_Position;

    uniform mat4 u_Model;

    void main() {
        gl_Position = u_Model * vec4(a_Position, 1.0);
    }

#pragma geometry

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

#pragma fragment

    #version 330 core

    in vec4 FragPos;

    uniform vec3 u_LightPosition;
    uniform float u_FarPlane;

    void main() {
        gl_FragDepth = length(FragPos.xyz - u_LightPosition) / u_FarPlane;
    }