#version 330 core

// In vals.
in vec2 TexCoords;

// Out vals.
out vec4 FragColor;

// Uniforms.
uniform sampler2D tex;
uniform vec4 color;

// Main method.
void main()
{
    FragColor = texture(tex, TexCoords) * color;
    if (FragColor.a == 0) discard;
}