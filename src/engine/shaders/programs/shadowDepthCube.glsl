#pragma vertex

    #version 330 core

    in vec3 a_Position;
    in vec3 a_Normal;
    in vec2 a_TexCoord;
    in vec3 a_Tangent;
    in vec3 a_Bitangent;

    struct VertexData {
        vec3 position;
        vec2 texCoord;
        vec3 normal;
        mat3 TBN;
    };

    out VertexData v;

    uniform mat4 u_Model;

    void main() {

        gl_Position = u_Model * vec4(a_Position, 1.0);

        // Position in model space:
        v.position = gl_Position.xyz;

        // Texture coordinates:
        v.texCoord = a_TexCoord;

        // Normal:
        v.normal = transpose(inverse(mat3(u_Model))) * a_Normal;

        // Compute TBN matrix:
        v.TBN = mat3(
            normalize(vec3(u_Model * vec4(a_Tangent,   0))),
            normalize(vec3(u_Model * vec4(a_Bitangent, 0))),
            normalize(vec3(u_Model * vec4(a_Normal,    0)))
        );
    }

#pragma geometry

    #version 330 core

    struct VertexData {
        vec3 position;
        vec2 texCoord;
        vec3 normal;
        mat3 TBN;
    };

    in  VertexData v[];
    out VertexData f;

    layout (triangles) in;
    layout (triangle_strip, max_vertices=18) out;

    uniform mat4 u_Matrices[6];

    void main() {

        for (int face = 0; face < 6; ++face) {

            // Built-in variable that specifies to which face we render.
            gl_Layer = face;

            // For each triangle vertex...
            for (int i = 0; i < 3; ++i) {

                f.position = v[i].position;
                f.texCoord = v[i].texCoord;
                f.normal   = v[i].normal;
                f.TBN      = v[i].TBN;

                gl_Position = u_Matrices[face] * gl_in[i].gl_Position;

                EmitVertex();
            }

            EndPrimitive();
        }
    }

#pragma fragment

    #version 330 core

    #extension GL_ARB_shading_language_include : require

    #include "/shaders/include/common_utils.glsl"

    struct VertexData {
        vec3 position;
        vec2 texCoord;
        vec3 normal;
        mat3 TBN;
    };

    in VertexData f;

    uniform vec3 u_LightPosition;
    uniform float u_FarPlane;

    uniform sampler2D u_Displacement;
    uniform float u_Displacement_Amount = 0.0;

    uniform vec4 u_ST = vec4(1.0, 1.0, 0.0, 0.0);

    void main() {

        vec3 viewDir_Tangent = normalize(
            (transpose(f.TBN) * normalize(u_LightPosition - f.position))
        );

        vec2 uvz = ParallaxMapping(
            u_Displacement,
            viewDir_Tangent,
            f.texCoord,
            u_ST,
            u_Displacement_Amount
        );

        vec3 fragPos = f.position - (f.normal * Sample1(u_Displacement, uvz.xy, u_ST) / u_Displacement_Amount);

        gl_FragDepth = length(fragPos - u_LightPosition) / u_FarPlane;
    }