#pragma once

#include "material.h"
#include "mesh.h"
#include "texture.h"
#include "vertexModes/vertexNormalUV.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#define COMBINED_CUBEMAP_NAME(left, right, top, bottom, front, back) left + ";" + right + ";" + top + ";" + bottom + ";" + front + ";" + back

// Model vertex type.
typedef VertexNormalUV JModelVertex;
typedef std::vector<std::tuple<std::string, std::string, std::string, std::string, std::string, std::string>> ModelCubemapTextures;

// Standard model.
struct JModel
{
    glm::mat4 matrix = glm::mat4(1.0f);
    glm::mat3 invTransposeMatrix = glm::mat3(1.0f);
    std::vector<std::unique_ptr<JMesh>> meshes;
    std::vector<std::unique_ptr<JTexture>> textures;
    std::vector<std::unique_ptr<JMaterial>> materials;
    JShader& shader;
    std::map<std::string, int> textureNameToTextureIndex; // Maps texture names to texture IDs.
    std::string relativeDirectory; // Used for loading textures from proper place.

    // Create a model from a singular mesh. Note that vertex attributes have to manually be set for each one.
    JModel(std::vector<std::unique_ptr<JMesh>>& meshes, std::vector<std::string> textureNames, std::vector<std::unique_ptr<JMaterial>>& materials, JShader& shader, glm::mat4 matrix = glm::mat4(1.0f));

    // Create a model from a singular mesh for a skybox. Note that vertex attributes have to manually be set for each one.
    JModel(std::vector<std::unique_ptr<JMesh>>& meshes, ModelCubemapTextures textureNames, std::vector<std::unique_ptr<JMaterial>>& materials, JShader& shader, glm::mat4 matrix = glm::mat4(1.0f));

    // Create from a model.
    JModel(std::string path, JShader& shader, glm::mat4 matrix = glm::mat4(1.0f));

    // Add a texture.
    void AddTexture(const std::string& name);

    // Add a cubemap texture.
    void AddTexture(const std::string& left, const std::string& right, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back);

    // Import a node.
    void ImportNode(const aiScene* scene, aiNode* node);

    // Import a mesh.
    void ImportMesh(const aiScene* scene, aiMesh* mesh);

    // Render a model.
    void Render(JShader* other = nullptr, unsigned int instanceCount = 0);

};