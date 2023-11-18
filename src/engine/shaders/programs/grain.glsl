#pragma vertex

    #version 330 core

    in vec2 a_Position;
    in vec2 a_TexCoord;

    out vec2 v_TexCoord;

    void main() {

      v_TexCoord = a_TexCoord;

      gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_explicit_uniform_location : enable

    in vec2 v_TexCoord;

    uniform sampler2D u_Texture;

    /* PARAMETERS */
    uniform float u_Amount;
    uniform float u_Time;

    // Gold Noise ©2015 dcerisano@standard3d.com
    // - based on the Golden Ratio
    // - uniform normalized distribution
    // - fastest static noise generator function (also runs at low precision)
    // - use with indicated fractional seeding method.

    float PHI = 1.61803398874989484820459;  // Φ = Golden Ratio

    float gold_noise(in vec2 xy, in float seed){
           return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
    }

    void main() {

        //float grain = gold_noise(v_TexCoord * 1000.0f, fract(u_Time));

        vec3 grain = vec3(
            gold_noise(v_TexCoord * 1000.0f, fract(u_Time) + 0.01),
            gold_noise(v_TexCoord * 1000.0f, fract(u_Time) + 0.02),
            gold_noise(v_TexCoord * 1000.0f, fract(u_Time) + 0.03)
        );

        gl_FragColor = vec4(texture2D(u_Texture, v_TexCoord).rgb - (grain * u_Amount), 1.0);
    }