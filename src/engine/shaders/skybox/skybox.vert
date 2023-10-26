#version 330

#extension GL_ARB_explicit_uniform_location: enable

layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoord;

/* PARAMETERS */
layout (location = 3) uniform mat4 u_Projection;
layout (location = 4) uniform mat4 u_Model;
layout (location = 5) uniform mat4 u_View;

void main() {

    v_TexCoord = vec3(u_Model * vec4(a_Position, 1.0));

    gl_Position = (u_Projection * u_View * u_Model * vec4(a_Position, 1.0)).xyww;
}