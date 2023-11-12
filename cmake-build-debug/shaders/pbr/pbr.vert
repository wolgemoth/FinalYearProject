#version 330 core

#extension GL_ARB_explicit_uniform_location: enable

in vec3 a_Position;
in vec2 a_TexCoord;
in vec3 a_Normal;

in vec3 a_Tangent;
in vec3 a_Bitangent;

out vec3 v_Position;
out vec2 v_TexCoord;
out vec3 v_Normal;

out vec4 v_Position_LightSpace;

out mat3 v_TBN;

/* PARAMETERS */
uniform mat4 u_Projection;
uniform mat4 u_Model;
uniform mat4 u_View;

uniform mat4 u_LightSpaceMatrix;

void main() {

    // Perform perspective projection on model vertex:
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

    // Position in model space:
    v_Position = vec3(u_Model * vec4(a_Position, 1.0));

    // Texture coordinates:
    v_TexCoord = a_TexCoord;

    // Normal:
    v_Normal = transpose(inverse(mat3((u_Model)))) * a_Normal;

    // Compute TBN matrix:
    v_TBN = mat3(
        normalize((u_Model * vec4(a_Tangent,   0)).xyz),
        normalize((u_Model * vec4(a_Bitangent, 0)).xyz),
        normalize((u_Model * vec4(a_Normal,    0)).xyz)
    );

    // Position in light space (for shadow calculations):
    v_Position_LightSpace = u_LightSpaceMatrix * vec4(v_Position, 1.0);
}