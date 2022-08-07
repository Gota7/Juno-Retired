#pragma once

#include "shader.h"
#include <glm/glm.hpp>

// Material for models.
struct JMaterial
{
    glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float specularExponent = 32.0f;

    // Set variables.
    void SetVars(JShader& shader, std::string materialName = "material");

};