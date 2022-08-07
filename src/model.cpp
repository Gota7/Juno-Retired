#include "model.h"
#include <glm/gtc/type_ptr.hpp>

void JModel::AddTexture(const std::string& name)
{
    if (textureNameToTextureIndex.find(name) == textureNameToTextureIndex.end())
    {
        int num = textureNameToTextureIndex.size();
        textures.push_back(std::make_unique<JTexture>(name));
        shader.SetInt("texture" + std::to_string(num), num);
        textures[num]->id = num;
        textureNameToTextureIndex[name] = num;
        num++;
    }
}

void JModel::ImportNode(const aiScene* scene, aiNode* node)
{

    // Import all meshes.
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ImportMesh(scene, mesh);
    }

    // Recursively search for other nodes.
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ImportNode(scene, node->mChildren[i]);
    }

}

void JModel::ImportMesh(const aiScene* scene, aiMesh* mesh)
{
}

void JModel::Render(JShader* other)
{
    JShader* toUse = &shader;
    if (other)
        toUse = other;
    toUse->Use();
    for (int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        textures[i]->Use();
    }
    toUse->SetMatrix("model", glm::value_ptr(matrix));
    for (auto& mesh : meshes)
    {
        materials[mesh->materialIndex]->SetVars(*toUse);
        mesh->Render();
    }
}