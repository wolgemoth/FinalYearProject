#pragma vertex

    #version 330 core

    layout (location = 0) in mediump vec2 a_Position;
    layout (location = 1) in mediump vec2 a_TexCoord;

    out mediump vec2 v_TexCoord;

    void main() {

        v_TexCoord = a_TexCoord;

        gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_texture_query_levels      : enable

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/common_utils.glsl"

    uniform sampler2D u_Texture0;
    uniform sampler2D u_Texture1;

    in mediump vec2 v_TexCoord;

    /* PARAMETERS */
    uniform mediump float u_Strength;

    void main() {

        // @Assessor: This shader is functionally-identical to the combine shader
        // which I submitted for my 3DGP assignment. I personally consider it to be
        // boilerplate code which is neccessary for the bloom effect, however you
        // may wish to exclude this from marking altogether.

        gl_FragColor = vec4(
            Sample3(u_Texture0, v_TexCoord) + (
                Sample3(u_Texture1, v_TexCoord) * u_Strength
            ),
            1.0
        );
    }