#pragma vertex

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    in mediump vec3 a_Position;

    out mediump vec4 v_Position;

    /* PARAMETERS */
    uniform mediump mat4 u_Projection;
    uniform mediump mat4 u_Model;
    uniform mediump mat4 u_View;

    void main() {

        // Perform perspective projection on model vertex:
        gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

        // Position in model space:
        v_Position = u_Model * vec4(a_Position, 1.0);
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"
    #include "/assets/shaders/core/include/common_utils.inc"

    in mediump vec4 v_Position;

    void main() {
        gl_FragColor = v_Position;
    }