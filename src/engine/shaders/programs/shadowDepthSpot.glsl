#pragma vertex

    #version 330 core

    in mediump vec3 a_Position;

    uniform mediump mat4 u_LightSpaceMatrix;
    uniform mediump mat4 u_Model;

    void main() {

        // @Assessor: This shader derived from an implementation by Learn OpenGL.
        // de Vries, J. (n.d.). LearnOpenGL - Shadow Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping [Accessed 15 Dec. 2023].

        gl_Position = u_LightSpaceMatrix * u_Model * vec4(a_Position, 1.0);
    }

#pragma fragment

    #version 330 core

    void main() {}