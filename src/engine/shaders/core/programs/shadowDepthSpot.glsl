#pragma vertex

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    in mediump vec3 a_Position;

    uniform mediump mat4 u_LightSpaceMatrix;
    uniform mediump mat4 u_Model;

    void main() {

        // This shader derived from an implementation by Learn OpenGL.
        // de Vries, J. (n.d.). LearnOpenGL - Shadow Mapping. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping [Accessed 15 Dec. 2023].

        gl_Position = u_LightSpaceMatrix * u_Model * vec4(a_Position, 1.0);
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    void main() {}