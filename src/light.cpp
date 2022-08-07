#include "light.h"
#include <glm/gtc/type_ptr.hpp>

void JLight::SetVars(JShader& shader, std::string lightName)
{
    shader.SetVec4(lightName + ".position", glm::value_ptr(glm::vec4(position, 1.0f)));
    shader.SetVec3(lightName + ".ambient", glm::value_ptr(ambient));
    shader.SetVec3(lightName + ".diffuse", glm::value_ptr(diffuse));
    shader.SetVec3(lightName + ".specular", glm::value_ptr(specular));
    shader.SetFloat(lightName + ".linear", linear);
    shader.SetFloat(lightName + ".quadratic", quadratic);
}

void JLightDirectional::SetVars(JShader& shader, std::string lightName)
{
    shader.SetVec4(lightName + ".position", glm::value_ptr(glm::vec4(direction, 0.0f)));
    shader.SetVec3(lightName + ".direction", glm::value_ptr(direction));
    shader.SetVec3(lightName + ".ambient", glm::value_ptr(ambient));
    shader.SetVec3(lightName + ".diffuse", glm::value_ptr(diffuse));
    shader.SetVec3(lightName + ".specular", glm::value_ptr(specular));
    shader.SetFloat(lightName + ".linear", 0.0f);
    shader.SetFloat(lightName + ".quadratic", 0.0f);
}

void JLightSpot::SetVars(JShader& shader, std::string lightName)
{
    shader.SetVec4(lightName + ".position", glm::value_ptr(glm::vec4(position, 0.5f)));
    shader.SetVec3(lightName + ".direction", glm::value_ptr(direction));
    shader.SetVec3(lightName + ".ambient", glm::value_ptr(ambient));
    shader.SetVec3(lightName + ".diffuse", glm::value_ptr(diffuse));
    shader.SetVec3(lightName + ".specular", glm::value_ptr(specular));
    shader.SetFloat(lightName + ".linear", linear);
    shader.SetFloat(lightName + ".quadratic", quadratic);
    shader.SetFloat(lightName + ".cutOff", cutOff);
    shader.SetFloat(lightName + ".outerCutOff", outerCutOff);
}