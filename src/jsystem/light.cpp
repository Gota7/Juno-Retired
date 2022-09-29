#include "light.h"
#include <glm/gtc/type_ptr.hpp>
#include <tracy/Tracy.hpp>

void JLightPoint::SetVars(JShader& shader, std::string lightName)
{
    ZoneScopedN("JLightPoint::SetVars");

    shader.SetVec3(lightName + ".position", glm::value_ptr(position));
    shader.SetVec3(lightName + ".ambient", glm::value_ptr(ambient));
    shader.SetVec3(lightName + ".diffuse", glm::value_ptr(diffuse));
    shader.SetVec3(lightName + ".specular", glm::value_ptr(specular));
    shader.SetFloat(lightName + ".linear", linear);
    shader.SetFloat(lightName + ".quadratic", quadratic);
}

void JLightDirectional::SetVars(JShader& shader, std::string lightName)
{
    ZoneScopedN("JLightDirectional::SetVars");

    shader.SetVec3(lightName + ".direction", glm::value_ptr(direction));
    shader.SetVec3(lightName + ".ambient", glm::value_ptr(ambient));
    shader.SetVec3(lightName + ".diffuse", glm::value_ptr(diffuse));
    shader.SetVec3(lightName + ".specular", glm::value_ptr(specular));
    shader.SetFloat(lightName + ".linear", 0.0f);
    shader.SetFloat(lightName + ".quadratic", 0.0f);
}

void JLightSpot::SetVars(JShader& shader, std::string lightName)
{
    ZoneScopedN("JLightSpot::SetVars");

    shader.SetVec3(lightName + ".position", glm::value_ptr(position));
    shader.SetVec3(lightName + ".direction", glm::value_ptr(direction));
    shader.SetVec3(lightName + ".ambient", glm::value_ptr(ambient));
    shader.SetVec3(lightName + ".diffuse", glm::value_ptr(diffuse));
    shader.SetVec3(lightName + ".specular", glm::value_ptr(specular));
    shader.SetFloat(lightName + ".linear", linear);
    shader.SetFloat(lightName + ".quadratic", quadratic);
    shader.SetFloat(lightName + ".cutOff", cutOff);
    shader.SetFloat(lightName + ".outerCutOff", outerCutOff);
}