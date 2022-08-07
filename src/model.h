#pragma once

#include "buffers.h"
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

// Standard model.
struct JModel
{
    glm::mat4 matrix = glm::mat4(1.0f);
    std::unique_ptr<JBuffers> buffers;
    std::vector<std::unique_ptr<JTexture>> textures = std::vector<std::unique_ptr<JTexture>>();
    JShader& shader;
    GLenum drawMode;
    GLsizei drawCount;
    GLenum drawType;

    JModel(
        void* vertexData,
        size_t vertexSize,
        GLenum vertexUsage,
        void* indexData,
        size_t indexSize,
        GLenum indexUsage,
        std::vector<std::string>& textures,
        JShader& shader,
        GLenum drawMode,
        GLsizei drawCount,
        GLenum drawType,
        glm::mat4 matrix = glm::mat4(1.0f)
    ) : buffers(std::make_unique<JBuffers>(vertexData, vertexSize, vertexUsage, indexData, indexSize, indexUsage)),
        matrix(matrix),
        shader(shader),
        drawMode(drawMode),
        drawCount(drawCount),
        drawType(drawType)
    {
        shader.Use();
        int num = 0;
        for (auto& tex : textures)
        {
            this->textures.push_back(std::make_unique<JTexture>(tex));
            shader.SetInt("texture" + std::to_string(num), num);
            this->textures[num]->id = num;
            num++;
        }
    }

    // Render a model.
    void Render();

};