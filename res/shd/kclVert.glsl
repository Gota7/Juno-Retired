#version 330 core

// Layouts.
layout (location = 0) in vec3 aPos;

// Uniforms.
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

// Out to fragment.
out vec3 FragPos;

// Main func.
void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
}