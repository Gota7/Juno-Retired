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
    std::vector<JModelVertex> vertices;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 pos = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        glm::vec3 normal;
        if (mesh->HasNormals())
        {
            normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
        else
        {
            normal = glm::vec3(0.0f);
        }
        glm::vec2 uv;
        if (mesh->HasTextureCoords(0))
        {
            uv = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            uv = glm::vec2(0.0f);
        }
        vertices.push_back(JModelVertex(pos, normal, uv));
    }
    size_t vertexSize = vertices.size() * sizeof(JModelVertex);
    std::vector<GLuint> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace* face = &mesh->mFaces[i];
        for(unsigned int j = 0; j < face->mNumIndices; j++)
            indices.push_back(face->mIndices[j]);
    }
    size_t indicesSize = indices.size() * sizeof(GLuint);
    meshes.push_back(std::make_unique<JMesh>(
        &vertices[0],
        vertexSize,
        GL_STATIC_DRAW,
        &indices[0],
        indicesSize,
        GL_STATIC_DRAW,
        GL_TRIANGLES,
        indices.size(),
        GL_UNSIGNED_INT,
        mesh->mMaterialIndex
    ));
    JModelVertex::SetAttributes();
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