#pragma once

#include "mesh.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

// Standard model.
struct JModel
{
    glm::mat4 matrix = glm::mat4(1.0f);
    std::vector<std::unique_ptr<JMesh>> meshes = std::vector<std::unique_ptr<JMesh>>();
    std::vector<std::unique_ptr<JTexture>> textures = std::vector<std::unique_ptr<JTexture>>();
    JShader& shader;

    // Create a model from a singular mesh.
    JModel(std::vector<std::unique_ptr<JMesh>>& meshes, std::vector<std::string> textureNames, JShader& shader, glm::mat4 matrix = glm::mat4(1.0f)) :
    matrix(matrix),
    meshes(std::move(meshes)),
    shader(shader)
    {
        shader.Use();
        int num = 0;
        for (auto& tex : textureNames)
        {
            textures.push_back(std::make_unique<JTexture>(tex));
            shader.SetInt("texture" + std::to_string(num), num);
            textures[num]->id = num;
            num++;
        }
        for (auto& mesh : this->meshes) {
            for (int i = 0; i < textureNames.size(); i++)
            {
                if (mesh->material->diffuseName == textureNames[i])
                {
                    mesh->material->diffuse = textures[i]->id;
                }
                else if (mesh->material->specularName == textureNames[i])
                {
                    mesh->material->specular = textures[i]->id;
                }
            }
        }
    }

    // Render a model.
    void Render();

};