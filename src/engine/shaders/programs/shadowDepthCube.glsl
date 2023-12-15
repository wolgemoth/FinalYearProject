#pragma vertex

    #version 330 core

    in mediump vec3 a_Position;

    uniform mediump mat4 u_Model;

    void main() {

        // @Assessor: This shader derived from an implementation by Learn OpenGL.
        // de Vries, J. (n.d.). LearnOpenGL - Point Shadows. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Point-Shadows [Accessed 15 Dec. 2023].

        gl_Position = u_Model * vec4(a_Position, 1.0);
    }

#pragma geometry

    #version 330 core

    layout (triangles) in;
    layout (triangle_strip, max_vertices=18) out;

    uniform mediump mat4 u_Matrices[6];

    out mediump vec4 FragPos; // FragPos from geometry shader. (Output per EmitVertex())

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

    in mediump vec4 FragPos;

    uniform mediump vec3 u_LightPosition;
    uniform mediump float u_FarPlane;

    void main() {
        gl_FragDepth = length(FragPos.xyz - u_LightPosition) / u_FarPlane;
    }