#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float specularExponent;
};

uniform Material material;
out vec4 FragColor;

void main()
{
    FragColor = vec4(material.diffuse, 1.0); // Ah why not, not like this is ever used outside debug.
}