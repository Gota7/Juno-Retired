#version 330 core

// Layouts.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// Uniforms.
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;
uniform mat3 invTransposeModel;
uniform vec3 viewPos;

// Out to fragment.
out vec4 EyeSpacePos;
out vec3 FragPos;
out vec3 Normal;
out vec3 viewDir;
out vec2 TexCoords;

// Main func.
void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    EyeSpacePos = view * vec4(FragPos, 1.0);
    gl_Position = projection * EyeSpacePos;
    Normal = normalize(invTransposeModel * aNormal);
    viewDir = normalize(viewPos - FragPos);
    TexCoords = aTexCoords;
}