#include "material.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void JMaterial::SetVars(JShader& shader, std::string materialName)
{
    shader.SetVec3(materialName + ".ambient", glm::value_ptr(ambient));
    shader.SetVec3(materialName + ".diffuse", glm::value_ptr(diffuse));
    shader.SetVec3(materialName + ".specular", glm::value_ptr(specular));
    shader.SetFloat(materialName + ".specularExponent", specularExponent);
}