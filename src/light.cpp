#include "light.h"
#include <glm/gtc/type_ptr.hpp>

void JLight::SetVars(JShader& shader, std::string lightName)
{
    shader.SetVec4(lightName + ".position", glm::value_ptr(glm::vec4(position, 1.0f)));
    shader.SetVec3(lightName + ".ambient", glm::value_ptr(ambient));
    shader.SetVec3(lightName + ".diffuse", glm::value_ptr(diffuse));
    shader.SetVec3(lightName + ".specular", glm::value_ptr(specular));
}

void JLightDirectional::SetVars(JShader& shader, std::string lightName)
{
    shader.SetVec4(lightName + ".position", glm::value_ptr(glm::vec4(direction, 0.0f)));
    shader.SetVec3(lightName + ".ambient", glm::value_ptr(ambient));
    shader.SetVec3(lightName + ".diffuse", glm::value_ptr(diffuse));
    shader.SetVec3(lightName + ".specular", glm::value_ptr(specular));
}