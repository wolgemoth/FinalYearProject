#pragma vertex

    #version 330 core

    layout (location = 0) in vec2 a_Position;
    layout (location = 1) in vec2 a_TexCoord;

    out vec2 v_TexCoord;

    void main() {

      v_TexCoord = a_TexCoord;

      gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    }

#pragma fragment

    #version 330 core

    #pragma vertex

    #extension GL_ARB_explicit_uniform_location : enable
    #extension GL_ARB_shading_language_include  : require

    #include "/shaders/include/rand.glsl"
    #include "/shaders/include/common_utils.glsl"
    #include "/shaders/include/lighting_utils.glsl"

    in vec2 v_TexCoord;

    uniform sampler2D u_Depth_gBuffer;
    uniform sampler2D u_Position_gBuffer;
    uniform sampler2D u_Normal_gBuffer;

    uniform int u_Samples = 32;

    uniform float u_Strength = 1.0;
    uniform float u_Bias = 0.025;

    uniform float u_NearClip;
    uniform float u_FarClip;

    uniform float u_Time;

    uniform mat4 u_Projection;

    void main() {

        vec3 position = Sample3(u_Position_gBuffer, v_TexCoord);
        vec3 normal   = Sample3(  u_Normal_gBuffer, v_TexCoord);
        float depth   = Sample1(u_Depth_gBuffer, v_TexCoord);

        float occlusion = 0.0;

        float radius = 0.1;

        for (int i = 0; i < u_Samples; i++) {

            vec3 randomVec = normalize(
                vec3(Random2(v_TexCoord * vec2(i + 1), u_Time, 0.2), 1.0)
            );

            vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
            vec3 bitangent = cross(normal, tangent);
            mat3 TBN       = mat3(tangent, bitangent, normal);

            vec3 samplePos = TBN * randomVec;
            samplePos = position + (samplePos * radius);

            vec4 pos = vec4(samplePos, 1.0); // make it a 4-vector
            pos = u_Projection * pos; // project on the near clipping plane
            pos.xy /= pos.w; // perform perspective divide
            pos.xy = pos.xy * 0.5 + vec2(0.5); // transform to (0,1) range

            float sampleDepth = texture(u_Position_gBuffer, pos.xy).b;

            gl_FragColor = vec4(abs(pos.xy), 0, 0);
            return;

            float rangeCheck = smoothstep(0.0, 1.0, radius / abs(position.z - depth));

            occlusion += (sampleDepth >= depth + u_Bias ? 0.0 : 1.0);
        }

        occlusion = 1.0 - ((occlusion / float(u_Samples)) * u_Strength);

        gl_FragColor = vec4(vec3(occlusion), 1.0);
    }