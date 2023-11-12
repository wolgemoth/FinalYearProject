#version 330 core

#extension GL_ARB_explicit_uniform_location: enable

in vec3 a_Position;
in vec2 a_TexCoord;
in vec3 a_Normal;

out vec3 v_Position;
out vec2 v_TexCoord;
out vec3 v_Normal;
out vec4 v_Position_LightSpace;

/* PARAMETERS */
uniform mat4 u_Projection;
uniform mat4 u_Model;
uniform mat4 u_View;

uniform mat4 u_LightSpaceMatrix;

void main() {

    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);

    v_Position = vec3(u_Model * vec4(a_Position, 1.0));
    v_TexCoord = a_TexCoord;
    v_Normal = transpose(inverse(mat3((u_Model)))) * a_Normal;

    v_Position_LightSpace = u_LightSpaceMatrix * vec4(v_Position, 1.0);
}