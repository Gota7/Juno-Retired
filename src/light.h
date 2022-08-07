#pragma once

#include "shader.h"
#include <glm/glm.hpp>
#include <string>

// Lighting structure.
struct JLight
{
    glm::vec3 position;
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

    // Set shader variables that corresponds to the light.
    void SetVars(JShader& shader, std::string lightName = "light");

};

// Lighting structure.
struct JLightDirectional
{
    glm::vec3 direction;
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

    // Constructor.
    JLightDirectional(glm::vec3 direction) : direction(direction) {}

    // Set shader variables that corresponds to the light.
    void SetVars(JShader& shader, std::string lightName = "light");

};