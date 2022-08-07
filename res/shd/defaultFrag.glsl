#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float specularExponent;
};

struct Light {
    vec4 position; // 0 for directional, 1 for positional.
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// In vals.
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

// Out vals.
out vec4 FragColor;

// Uniforms.
uniform Light light;
uniform Material material;
uniform vec3 viewPos;

// Main method.
void main()
{

    // Ambient.
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // Diffuse.
    vec3 norm = normalize(Normal);
    vec3 lightDir;
    if (light.position.w == 0.0)
        lightDir = normalize(-light.position.xyz);
    else if (light.position.w == 1.0)
        lightDir = normalize(light.position.xyz - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // Specular.
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularExponent);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);

}