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

    #extension GL_ARB_explicit_uniform_location : enable
    #extension GL_ARB_texture_query_levels      : enable

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/common_utils.glsl"

    uniform sampler2D u_Texture;

    in mediump vec2 v_TexCoord;

    /* PARAMETERS */
    uniform mediump float u_Threshold;
    uniform mediump float u_Clamp;

    void main() {

        // @Assessor: This shader is functionally-identical to the threshold shader
        // which I submitted for my 3DGP assignment. I personally consider it to be
        // boilerplate code which is necessary for the bloom effect, however you may
        // wish to exclude this from marking altogether.

        gl_FragColor = min(
            vec4(
                max(
                    Sample3(u_Texture, v_TexCoord) - vec3(u_Threshold),
                    vec3(0.0)
                ),
                1.0
            ),
            vec4(vec3(u_Clamp), 1.0)
        );
    }