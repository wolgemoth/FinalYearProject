#pragma vertex

    #version 330 core

    #extension GL_ARB_explicit_uniform_location : enable

    layout (location = 0) in mediump vec3 a_Position;

    out mediump vec3 v_TexCoord;

    /* PARAMETERS */
    layout (location = 3) uniform mediump mat4 u_Projection;
    layout (location = 4) uniform mediump mat4 u_Model;
    layout (location = 5) uniform mediump mat4 u_View;

    void main() {

        v_TexCoord = vec3(u_Model * vec4(a_Position, 1.0));

        gl_Position = (u_Projection * u_View * u_Model * vec4(a_Position, 1.0)).xyww;
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_explicit_uniform_location : enable
    #extension GL_ARB_texture_query_levels      : enable

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/common_utils.inc"

    in mediump vec3 v_TexCoord;

    //#define SAMPLER_CUBE

    #ifdef SAMPLER_CUBE
        layout (location = 0) uniform samplerCube u_Texture;
    #else
        layout (location = 0) uniform sampler2D u_Texture;
    #endif

    uniform mediump float u_Exposure = 1.0;
    uniform mediump float u_Blur     = 0.0;

    void main() {
        gl_FragColor = vec4(SampleAmbient(u_Texture, v_TexCoord, 0.0), 1.0) * u_Exposure;
    }