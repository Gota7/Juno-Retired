#pragma once

#include "buffers.h"
#include "material.h"
#include "shader.h"
#include "texture.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

// Standard mesh.
struct JMesh
{
    std::unique_ptr<JBuffers> buffers;
    GLenum drawMode;
    GLsizei drawCount;
    GLenum drawType;
    std::unique_ptr<JMaterialTex> material;

    JMesh(
        void* vertexData,
        size_t vertexSize,
        GLenum vertexUsage,
        void* indexData,
        size_t indexSize,
        GLenum indexUsage,
        GLenum drawMode,
        GLsizei drawCount,
        GLenum drawType,
        std::unique_ptr<JMaterialTex>& material
    ) : buffers(std::make_unique<JBuffers>(vertexData, vertexSize, vertexUsage, indexData, indexSize, indexUsage)),
        drawMode(drawMode),
        drawCount(drawCount),
        drawType(drawType),
        material(std::move(material))
    {}

    // Render a model.
    void Render();

};