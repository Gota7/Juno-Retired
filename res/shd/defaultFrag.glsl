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

struct Fog {
    vec3 color;
    float start;
    float end;
    float density;
    int type;
    bool enabled;
};

// In vals.
in vec4 EyeSpacePos;
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
uniform Fog fog;

// calculates the color when using a directional light.
vec4 CalcDirLight(LightDirectional light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading.
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading.
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.specularExponent);

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
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.specularExponent);

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
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.specularExponent);

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

// Calculate fog.
float getFogFactor(float fogCoordinate)
{
	float result = 0.0;
	if (fog.type == 0)
	{
		float fogLength = fog.end - fog.start;
		result = (fog.end - fogCoordinate) / fogLength;
	}
	else if (fog.type == 1)
    {
		result = exp(-fog.density * fogCoordinate);
	}
	else if (fog.type == 2)
    {
		result = exp(-pow(fog.density * fogCoordinate, 2.0));
	}
	
	result = 1.0 - clamp(result, 0.0, 1.0);
	return result;
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
    if (fog.enabled)
    {
        float fogCoordinate = abs(EyeSpacePos.z / EyeSpacePos.w);
        FragColor = mix(result, vec4(fog.color, 1.0), getFogFactor(fogCoordinate));
    }
    else
    {
        FragColor = result;
    }
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}