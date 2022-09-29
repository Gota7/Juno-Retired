#include "material.h"
#include <glm/gtc/type_ptr.hpp>
#include <tracy/Tracy.hpp>

void JMaterialSolid::SetVars(JShader& shader, std::string materialName)
{
    ZoneScopedN("JMaterialSolid::SetVars");
    shader.SetVec3(materialName + ".ambient", glm::value_ptr(ambient));
    shader.SetVec3(materialName + ".diffuse", glm::value_ptr(diffuse));
    shader.SetVec3(materialName + ".specular", glm::value_ptr(specular));
    shader.SetFloat(materialName + ".specularExponent", specularExponent);
}

void JMaterialTex::SetVars(JShader& shader, std::string materialName)
{
    ZoneScopedN("JMaterialTex::SetVars");
    shader.SetInt(materialName + ".diffuse", diffuse);
    shader.SetInt(materialName + ".specular", specular);
    shader.SetFloat(materialName + ".specularExponent", specularExponent);
}