#pragma vertex

    #version 330 core

    in vec3 a_Position;

    uniform mat4 u_LightSpaceMatrix;
    uniform mat4 u_Model;

    void main() {
        gl_Position = u_LightSpaceMatrix * u_Model * vec4(a_Position, 1.0);
    }

#pragma fragment

    #version 330 core

    void main() {

    }