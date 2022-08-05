#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 ourColor;
out vec2 TexCoord;
uniform mat4 model;
void main()
{
    gl_Position = model * vec4(aPos, 1.0);
    ourColor = aColor;
    TexCoord = aTexCoord;
}