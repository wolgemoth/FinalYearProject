#version 330 core

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

/* PARAMETERS */
uniform float u_Gain     = 0.0;
uniform float u_Exposure = 1.0;

vec3 ACES(vec3 _hdr) {

    mat3x3 aInput;
    mat3x3 aOutput;

    // ACES INPUT MAT
    aInput[0] = vec3(0.59719, 0.35458, 0.04823);
    aInput[1] = vec3(0.07600, 0.90834, 0.01566);
    aInput[2] = vec3(0.02840, 0.13383, 0.83777);

    _hdr = _hdr * aInput;

    // RTT ODT FIT
    vec3 a = _hdr * (_hdr + 0.0245786) - 0.000090537;
    vec3 b = _hdr * (0.983729 * _hdr + 0.4329510) + 0.238081;
    _hdr = a / b;
    
    // ACES OUTPUT MAT
    aOutput[0] = vec3( 1.60475, -0.53108, -0.07367);
    aOutput[1] = vec3(-0.10208,  1.10813, -0.00605);
    aOutput[2] = vec3(-0.00327, -0.07276,  1.07602);
    
    return _hdr * aOutput;
}

void main() {

    vec3 tonemapped = ACES(vec3(texture2D(u_Texture, v_TexCoord) * u_Exposure) + u_Gain);

	gl_FragColor = vec4(tonemapped, 1.0);
}
