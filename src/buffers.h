#pragma once

#include <glad/glad.h>
#include <stddef.h>
#include <vector>

#define EMPTY_BUFFER VertexBuffer(0, 0, 0)

typedef GLuint VBO, VAO, EBO;
struct VertexBuffer
{
    VAO vertexArray;
    VBO vertexBuffer;
    EBO elementBuffer;

    VertexBuffer() {}
    VertexBuffer(VAO vertexArray, VBO vertexBuffer, EBO elementBuffer)
    {
        this->vertexArray = vertexArray;
        this->vertexBuffer = vertexBuffer;
        this->elementBuffer = elementBuffer;
    }
};

// Generate vertex buffers.
VertexBuffer Buffers_Generate(void* vertexData, size_t vertexSize, GLenum vertexUsage, void* indexData, size_t indexSize, GLenum indexUsage);

// Bind a vertex buffer.
void Buffers_Bind(VertexBuffer buffers);

// Delete vertex buffers.
void Buffers_Delete(VertexBuffer buffers);

// Buffers object.
struct JBuffers : VertexBuffer
{
    JBuffers(void* vertexData, size_t vertexSize, GLenum vertexUsage, void* indexData, size_t indexSize, GLenum indexUsage);
    void Bind();
    ~JBuffers();
};