#include "model.h"
#include <glm/gtc/type_ptr.hpp>
#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>

JModel::JModel(std::vector<std::unique_ptr<JMesh>>& meshes, std::vector<std::string> textureNames, std::vector<std::unique_ptr<JMaterial>>& materials, JShader& shader, glm::mat4 matrix) :
matrix(matrix),
meshes(std::move(meshes)),
materials(std::move(materials)),
shader(shader)
{
    ZoneScopedN("JModel::JModel");

    shader.Use();
    if (textureNames.size() > 0)
    {
        for (auto& tex : textureNames)
        {
            AddTexture(tex);
        }
        for (auto& mat : this->materials) {
            JMaterialTex* m = static_cast<JMaterialTex*>(mat.get());
            m->diffuse = textureNameToTextureIndex[m->diffuseName];
            m->specular = textureNameToTextureIndex[m->specularName];
        }
    }
}

JModel::JModel(std::vector<std::unique_ptr<JMesh>>& meshes, ModelCubemapTextures textureNames, std::vector<std::unique_ptr<JMaterial>>& materials, JShader& shader, glm::mat4 matrix) :
matrix(matrix),
meshes(std::move(meshes)),
materials(std::move(materials)),
shader(shader)
{
    ZoneScopedN("JModel::JModel");

    shader.Use();
    if (textureNames.size() > 0)
    {
        for (auto& tex : textureNames)
        {
            AddTexture(std::get<0>(tex), std::get<1>(tex), std::get<2>(tex), std::get<3>(tex), std::get<4>(tex), std::get<5>(tex));
        }
        for (auto& mat : this->materials) {
            JMaterialTex* m = static_cast<JMaterialTex*>(mat.get());
            m->diffuse = textureNameToTextureIndex[m->diffuseName];
            m->specular = textureNameToTextureIndex[m->specularName];
        }
    }
}

JModel::JModel(std::string path, JShader& shader, glm::mat4 matrix) : shader(shader), matrix(matrix)
{
    ZoneScopedN("JModel::JModel");

    // Initial setup.
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

    // Add null texture.
    const std::string nullTex = "res/tex/null.png";
    AddTexture(nullTex);

    // Add materials.
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* mat = scene->mMaterials[i];
        std::string diffuseName = nullTex;
        std::string specularName = nullTex;
        if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) // Only assume 1 texture.
        {
            aiString str;
            mat->GetTexture(aiTextureType_DIFFUSE, 0, &str);
            std::string texName = relativeDirectory + str.C_Str();
            AddTexture(texName);
            diffuseName = texName;
        }
        if (mat->GetTextureCount(aiTextureType_SPECULAR) > 0) // Only assume 1 texture.
        {
            aiString str;
            mat->GetTexture(aiTextureType_SPECULAR, 0, &str);
            std::string texName = relativeDirectory + str.C_Str();
            AddTexture(texName);
            specularName = texName;
        }
        materials.push_back(std::make_unique<JMaterialTex>(diffuseName, specularName));
    }
    for (auto& mat : this->materials) {
        JMaterialTex* m = static_cast<JMaterialTex*>(mat.get());
        m->diffuse = textureNameToTextureIndex[m->diffuseName];
        m->specular = textureNameToTextureIndex[m->specularName];
    }

    // Import root node.
    ImportNode(scene, scene->mRootNode);

}

void JModel::AddTexture(const std::string& name)
{
    ZoneScopedN("JModel::AddTexture");

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

void JModel::AddTexture(const std::string& left, const std::string& right, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back)
{
    ZoneScopedN("JModel::AddTexture");

    std::string combinedName = COMBINED_CUBEMAP_NAME(left, right, top, bottom, front, back);
    if (textureNameToTextureIndex.find(combinedName) == textureNameToTextureIndex.end())
    {
        int num = textureNameToTextureIndex.size();
        textures.push_back(std::make_unique<JTexture>(left, right, top, bottom, front, back));
        shader.SetInt("texture" + std::to_string(num), num);
        textures[num]->id = num;
        textureNameToTextureIndex[combinedName] = num;
        num++;
    }
}

void JModel::ImportNode(const aiScene* scene, aiNode* node)
{
    ZoneScopedN("JModel::ImportNode");

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
    ZoneScopedN("JModel::ImportMesh");

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

void JModel::Render(JShader* other, unsigned int instanceCount)
{
    ZoneScopedN("JModel::Render");
    TracyGpuZone("JModel::Render");

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
        mesh->Render(instanceCount);
    }
}