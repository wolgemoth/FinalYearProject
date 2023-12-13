#pragma vertex

    #version 330

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

    #version 330

    #extension GL_ARB_explicit_uniform_location : enable
    #extension GL_ARB_texture_query_levels      : enable

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/common_utils.glsl"

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

        vec3 result;

        float      s = textureSize(u_Texture, 0).x;
        float levels = log2(s);

        int b = int(pow(u_Blur, 2.0f) * levels);

    #if SAMPLER_CUBE

        // Sample the cubemap using the direction directly.
        result = Sample3(u_Texture, v_TexCoord, b);

    #else

        vec3 d = normalize(v_TexCoord);

        // Sample the texture2d by converting the direction to a uv coordinate.
        // See the example given on: https://en.wikipedia.org/wiki/UV_mapping

        vec2 uv = vec2(
            0.5 + ((atan(d.z, d.x) / PI) / 2.0),
            0.5 - (asin(d.y) / PI)
        );

        // Fix seam at wrap-around point.
        float threshold = 0.0005;

        float check = step(fract(uv.x - threshold), 1.0 - (threshold * 2.0));

        //result = Sample3(u_Texture, vec2(uv.x * check, uv.y), b);

        // Seemingly no way to do this without a branch.
        // Compiler seems to need an explicit branch on
        // texture access to 'get it'.
        //
        // See for yourself by commenting out the following code
        // and uncommenting the previous code.

        result = check > 0.0 ?
            Sample3(u_Texture, uv, b) :
            Sample3(u_Texture, vec2(0.0, uv.y), b);

    #endif

        gl_FragColor = vec4(result, 1.0) * u_Exposure;
    }