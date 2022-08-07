#pragma once

#include "material.h"
#include "mesh.h"
#include "texture.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

// Standard model.
struct JModel
{
    glm::mat4 matrix = glm::mat4(1.0f);
    std::vector<std::unique_ptr<JMesh>> meshes = std::vector<std::unique_ptr<JMesh>>();
    std::vector<std::unique_ptr<JTexture>> textures = std::vector<std::unique_ptr<JTexture>>();
    std::vector<std::unique_ptr<JMaterialTex>> materials = std::vector<std::unique_ptr<JMaterialTex>>();
    JShader& shader;
    std::map<std::string, int> textureNameToTextureIndex;
    std::string relativeDirectory; // Used for loading textures from proper place.

    // Create a model from a singular mesh.
    JModel(std::vector<std::unique_ptr<JMesh>>& meshes, std::vector<std::string> textureNames, std::vector<std::unique_ptr<JMaterialTex>>& materials, JShader& shader, glm::mat4 matrix = glm::mat4(1.0f)) :
    matrix(matrix),
    meshes(std::move(meshes)),
    materials(std::move(materials)),
    shader(shader)
    {
        shader.Use();
        for (auto& tex : textureNames)
        {
            AddTexture(tex);
        }
        for (auto& mat : this->materials) {
            mat->diffuse = textureNameToTextureIndex[mat->diffuseName];
            mat->specular = textureNameToTextureIndex[mat->specularName];
        }
    }

    // Create a model from a file.
    JModel(std::string path, JShader& shader, glm::mat4 matrix = glm::mat4(1.0f)) : shader(shader), matrix(matrix)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_OptimizeMeshes);
        relativeDirectory = path.substr(0, path.find_last_of('/'));
        if (relativeDirectory != "")
            relativeDirectory += "/";
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        ImportNode(scene, scene->mRootNode);
    }

    // Add a texture.
    void AddTexture(const std::string& name);

    // Import a node.
    void ImportNode(const aiScene* scene, aiNode* node);

    // Import a mesh.
    void ImportMesh(const aiScene* scene, aiMesh* mesh);

    // Render a model.
    void Render(JShader* other = nullptr);

};