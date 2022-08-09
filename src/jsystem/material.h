#pragma once

#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <memory>

// Standard material.
/*struct JMaterial
{
    virtual void SetVars(JShader& shader, std::string materialName = "material") {}
};*/

// Solid material for models.
struct JMaterialSolid //: JMaterial
{
    glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float specularExponent = 32.0f;

    // Set variables.
    void SetVars(JShader& shader, std::string materialName = "material");

};

// Material for models.
struct JMaterialTex //: JMaterial
{
    int diffuse;
    int specular;
    float specularExponent = 32.0f;
    std::string diffuseName;
    std::string specularName;

    // Constructor.
    JMaterialTex(std::string diffuseName, std::string specularName) : diffuseName(diffuseName), specularName(specularName) {}

    // Set variables.
    void SetVars(JShader& shader, std::string materialName = "material");

};