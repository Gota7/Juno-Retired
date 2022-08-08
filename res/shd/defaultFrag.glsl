#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float specularExponent;
};

struct LightPoint {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float linear;
    float quadratic;
};

struct LightDirectional {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct LightSpot {
    vec3 position;
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
uniform LightPoint lightPoint;
uniform LightDirectional lightDirectional;
uniform LightSpot lightSpot;
uniform Material material;
uniform vec3 viewPos;

// calculates the color when using a directional light.
vec4 CalcDirLight(LightDirectional light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading.
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading.
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularExponent);

    // Combine results.
    vec4 ambient = vec4(light.ambient, 1.0) * texture(material.diffuse, TexCoords);
    vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(material.diffuse, TexCoords);
    vec4 specular = vec4(light.specular, 1.0) * spec * texture(material.specular, TexCoords);
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec4 CalcPointLight(LightPoint light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading.
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading.
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularExponent);

    // Attenuation.
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));

    // Combine results.
    vec4 ambient = vec4(light.ambient, 1.0) * texture(material.diffuse, TexCoords);
    vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(material.diffuse, TexCoords);
    vec4 specular = vec4(light.specular, 1.0) * spec * texture(material.specular, TexCoords);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec4 CalcSpotLight(LightSpot light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse shading.
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading.
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.specularExponent);

    // Attenuation.
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * (distance * distance));

    // Spotlight intensity.
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Combine results.
    vec4 ambient = vec4(light.ambient, 1.0) * texture(material.diffuse, TexCoords);
    vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(material.diffuse, TexCoords);
    vec4 specular = vec4(light.specular, 1.0) * spec * texture(material.specular, TexCoords);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

// Main method.
void main()
{
    // Properties.
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec4 result = CalcDirLight(lightDirectional, norm, viewDir);
    result += CalcPointLight(lightPoint, norm, FragPos, viewDir);
    result += CalcSpotLight(lightSpot, norm, FragPos, viewDir);
    FragColor = result;
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}