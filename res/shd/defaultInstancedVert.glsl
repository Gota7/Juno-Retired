#version 330 core

// Layouts.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aOffset;

// Uniforms.
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;

// Out to fragment.
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

// Main func.
void main()
{
    FragPos = vec3(model * vec4(aPos + aOffset, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal; // Note: Do the matrix creation before hand on CPU, inverse is expensive for shaders!
    TexCoords = aTexCoords;
}