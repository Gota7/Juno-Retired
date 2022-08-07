#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float specularExponent;
};

struct Light {
    vec4 position; // 0 for directional, 0.5 for spotlight, and 1 for positional.
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
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
    if (light.position.w == 0.0) // Directional.
        lightDir = normalize(-light.direction);
    else
        lightDir = normalize(light.position.xyz - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

    // Specular.
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularExponent);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    // Spotlights.
    if (light.position.w == 0.5) {
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = (light.cutOff - light.outerCutOff);
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;
    }

    // Attenuation.
    float lightDist = length(light.position.xyz - FragPos);
    float attenuation = 1.0 / (1.0 + light.linear * lightDist + light.quadratic * (lightDist * lightDist));

    vec3 result = (ambient + diffuse + specular) * attenuation;
    FragColor = vec4(result, 1.0);

}