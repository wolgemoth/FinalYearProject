#pragma vertex

    #version 330 core

    /* PARAMETERS */
    layout (location = 0) in mediump vec2 a_Position;
    layout (location = 1) in mediump vec2 a_TexCoord;

    out mediump vec2 v_TexCoord;

    void main() {

        v_TexCoord = a_TexCoord;

        gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_explicit_uniform_location : enable
    #extension GL_ARB_texture_query_levels      : enable

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/common_utils.glsl"

    uniform sampler2D u_Texture;

    in mediump vec2 v_TexCoord;

    void main() {

        // @Assessor: This shader is functionally-identical to the passthrough shader
        // which I submitted for my 3DGP assignment. I personally consider it to be
        // boilerplate code applicable to any similar graphics engine, however you may
        // wish to exclude this from marking altogether.
        gl_FragColor = Sample4(u_Texture, v_TexCoord);
    }