#version 330

#extension GL_ARB_texture_query_levels      : enable
#extension GL_ARB_explicit_uniform_location : enable

const float PI = 3.141593;

in vec3 v_TexCoord;

//#define SAMPLER_CUBE

#ifdef SAMPLER_CUBE
    layout (location = 0) uniform samplerCube u_Texture;
#else
    layout (location = 0) uniform sampler2D u_Texture;
#endif

uniform float u_Exposure = 1.0;
uniform float u_Blur     = 0.0;

vec3 SampleAmbient(in vec3 _dir) {

    vec3 result;

    int mipLevel = int(float(textureQueryLevels(u_Texture)) * u_Blur);

    #ifdef SAMPLER_CUBE

        // Sample the cubemap the direction directly.
        result = texture(u_Texture, _dir, mipLevel).rgb;
    #else

        vec3 d = normalize(_dir);

        // Sample the texture2d by converting the direction to a uv coordinate.
        // See the example given on: https://en.wikipedia.org/wiki/UV_mapping
        result = texture(
            u_Texture,
            vec2(
                0.5 + ((atan(d.z, d.x) / PI) / 2.0),
                0.5 - (asin(d.y) / PI)
            ),
            mipLevel
        ).rgb;

    #endif

    return result * u_Exposure;
}

void main() {
    gl_FragColor = vec4(SampleAmbient(v_TexCoord), 1.0);
}