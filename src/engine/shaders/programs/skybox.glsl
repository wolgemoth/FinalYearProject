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

    #extension GL_ARB_texture_query_levels      : enable
    #extension GL_ARB_explicit_uniform_location : enable

    const mediump float PI = 3.141593;

    in mediump vec3 v_TexCoord;

    //#define SAMPLER_CUBE

    #ifdef SAMPLER_CUBE
        layout (location = 0) uniform samplerCube u_Texture;
    #else
        layout (location = 0) uniform sampler2D u_Texture;
    #endif

    uniform mediump float u_Exposure = 1.0;
    uniform mediump float u_Blur     = 0.0;

    mediump vec3 SampleAmbient(in vec3 _dir) {

        mediump vec3 result;

        mediump float s = textureSize(u_Texture, 0).x;

        mediump float levels = log2(s);

        int b = int(pow(u_Blur, 2.0f) * levels);

        #ifdef SAMPLER_CUBE

            // Sample the cubemap the direction directly.
            result = texture(u_Texture, _dir, b).rgb;
        #else

            mediump vec3 d = normalize(_dir);

            // Sample the texture2d by converting the direction to a uv coordinate.
            // See the example given on: https://en.wikipedia.org/wiki/UV_mapping

            mediump vec2 uv = vec2(
                0.5 + ((atan(d.z, d.x) / PI) / 2.0),
                0.5 - (asin(d.y) / PI)
            );

            // Fix seam at wrap-around point.
            mediump float threshold = 4.0 / textureSize(u_Texture, b).x;

            mediump float check = step(fract(uv.x - threshold), 1.0 - (threshold * 2.0));

            //result = texture(u_Texture, vec2(uv.x * check, uv.y), b).rgb;

            // Seemingly no way to do this without a branch.
            // Compiler seems to need an explicit branch to 'get it'.
            //
            // See for yourself by commenting out the following code
            // and uncommenting the previous code.

            result = check > 0 ?
                texture(u_Texture, uv, b).rgb :
                texture(u_Texture, vec2(0.0, uv.y), b).rgb;

        #endif

        return result * u_Exposure;
    }

    void main() {
        gl_FragColor = vec4(SampleAmbient(v_TexCoord), 1.0);
    }