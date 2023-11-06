#version 330

const float PI = 3.141593;

#define SAMPLER_CUBE

in vec3 v_TexCoord;

#ifdef SAMPLER_CUBE
    uniform samplerCube u_Texture;
#else
    uniform sampler2D u_Texture;
#endif

uniform float u_Exposure = 1.0;

vec3 Sample(in vec3 _dir) {

    vec3 result;

    #ifdef SAMPLER_CUBE

        // Sample the cubemap the direction directly.
        result = texture(u_Texture, _dir).rgb;
    #else

        // Sample the 2D texture by converting the direction to uv coordinates.
        result = texture(u_Texture,
            vec2(
                0.5 + (0.5 * atan(_dir.z, _dir.x) / PI),
                1.0 - (acos(_dir.y) / PI)
            )
        ).rgb;

    #endif

    return result * u_Exposure;
}

void main() {
    gl_FragColor = vec4(Sample(v_TexCoord), 1.0);
}