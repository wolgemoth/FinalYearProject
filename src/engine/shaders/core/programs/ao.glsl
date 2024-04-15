#pragma vertex

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    layout (location = 0) in mediump vec2 a_Position;
    layout (location = 1) in mediump vec2 a_TexCoord;

    out mediump vec2 v_TexCoord;

    void main() {

        v_TexCoord = a_TexCoord;

        gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
    }

#pragma fragment

    #version 330 core

    #pragma vertex

    #extension GL_ARB_explicit_uniform_location : enable
    #extension GL_ARB_shading_language_include  : require

    #include "/assets/shaders/core/include/strict_optimisation_flags.inc"

    #include "/assets/shaders/core/include/rand.inc"
    #include "/assets/shaders/core/include/common_utils.inc"
    #include "/assets/shaders/core/include/lighting_utils.inc"

    in mediump vec2 v_TexCoord;

    uniform sampler2D u_Depth_gBuffer;
    uniform sampler2D u_Position_gBuffer;
    uniform sampler2D u_Normal_gBuffer;

    uniform int u_Samples = 32;

    uniform mediump float u_Strength =  1.0;
    uniform mediump float u_Bias     = -0.2;
    uniform mediump float u_Radius   =  0.2;

    uniform mediump float u_NearClip;
    uniform mediump float u_FarClip;

    uniform mediump float u_Time;

    uniform mediump mat4 u_VP;

    void main() {

        // de Vries, J. (n.d.). LearnOpenGL - SSAO. [online] learnopengl.com. Available at: https://learnopengl.com/Advanced-Lighting/SSAO [Accessed 15 Dec. 2023].

        mediump vec3  position = Sample3(u_Position_gBuffer, v_TexCoord);
        mediump vec3  normal   = Sample3(  u_Normal_gBuffer, v_TexCoord);
        mediump float depth    = Linear01Depth(Sample1(u_Depth_gBuffer, v_TexCoord), u_NearClip, u_FarClip) * u_FarClip;

        mediump float occlusion = 0.0;

        for (int i = 0; i < u_Samples; ++i) {

            mediump vec3 randomVec = normalize(
                vec3((Random2S(v_TexCoord + vec2(i + 1), u_Time, 0.0) + 1.0) / 2, 1.0)
            );

            mediump vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
            mediump vec3 bitangent = cross(normal, tangent);
            mediump mat3 TBN       = mat3(tangent, bitangent, normal);

            randomVec = (TBN * ((randomVec * 2.0) - 1.0)) * u_Radius;

            mediump vec3 samplePosition = position + randomVec;

            mediump vec4 offsetUV      = vec4(samplePosition, 1.0);
                 offsetUV      = u_VP * offsetUV;
                 offsetUV.xyz /= offsetUV.w;
                 offsetUV.xy   = offsetUV.xy * 0.5 + 0.5;

            mediump float sampleDepth = Linear01Depth(Sample1(u_Depth_gBuffer, offsetUV.xy), u_NearClip, u_FarClip) * u_FarClip;

            mediump float rangeCheck = smoothstep(0.0, 1.0, u_Radius / abs(sampleDepth - depth));

            occlusion += (sampleDepth >= depth + u_Bias ? 0.0 : 1.0) * rangeCheck;
        }

        occlusion = 1.0 - ((occlusion / float(u_Samples)) * u_Strength);

        gl_FragColor = vec4(vec3(occlusion), 1.0);
    }