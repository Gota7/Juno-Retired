#version 330 core

// Layouts.
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

// Uniforms.
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};
uniform mat4 model;
uniform int drawMode;

// Out to fragment.
out vec2 TexCoords;

// Main func.
void main()
{
    if (drawMode == 0 || drawMode == 2) // 3d.
    {
        vec3 FragPos = vec3(model * vec4(aPos, 1.0));
        gl_Position = projection * view * vec4(FragPos, 1.0);
    }
    else if (drawMode == 1) // Stretch.
    {

    }
    else // Billboard.
    {
        mat4 modelView = view * model;
        modelView[0][0] = 1;
        modelView[0][1] = 0;
        modelView[0][2] = 0;
        modelView[1][0] = 0;
        modelView[1][1] = 1;
        modelView[1][2] = 0;
        modelView[2][0] = 0;
        modelView[2][1] = 0;
        modelView[2][2] = 1;
        gl_Position = projection * modelView * vec4(aPos, 1.0);
    }
    TexCoords = aTexCoords;
}