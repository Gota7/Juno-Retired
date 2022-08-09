#pragma once

#include "shader.h"
#include <glm/glm.hpp>
#include <string>

// General light that is at a point and gives light off in every direction.
struct JLightPoint
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float linear = 0.09f;
    float quadratic = 0.032f;

    // Set shader variables that corresponds to the light.
    void SetVars(JShader& shader, std::string lightName = "lightPoint");

};

// Global light like sun that is applied to every object as if it is coming from a direction.
struct JLightDirectional
{
    glm::vec3 direction;
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

    // Constructor.
    JLightDirectional(glm::vec3 direction) : direction(direction) {}

    // Set shader variables that corresponds to the light.
    void SetVars(JShader& shader, std::string lightName = "lightDirectional");

};

// Light that covers a certain range and nothing else, like a flashlight.
struct JLightSpot
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction;
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float linear = 0.09f;
    float quadratic = 0.032f;
    float cutOff = glm::cos(glm::radians(12.5f));
    float outerCutOff = glm::cos(glm::radians(17.5f));

    // Constructor.
    JLightSpot(glm::vec3 direction) : direction(direction) {}

    // Set shader variables that corresponds to the light.
    void SetVars(JShader& shader, std::string lightName = "lightSpot");

};