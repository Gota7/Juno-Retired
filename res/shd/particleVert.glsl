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
uniform float aspect;

// Out to fragment.
out vec2 TexCoords;

// Main func.
void main()
{
    if (drawMode == 0) // 3d.
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
        vec2 scale = vec2(
            length(modelView[0]) / aspect,
            length(modelView[1])
        );

        vec4 billboard = (modelView * vec4(vec3(0.0), 1.0));
        vec4 newPosition = projection
            * billboard
            + vec4(scale * aPos.xy, 0.0, 0.0);

        gl_Position = newPosition;
        //vec2 vUv = vec2(aPos.x + 0.5, aPos.y + 0.5);
    }
    TexCoords = aTexCoords;
}