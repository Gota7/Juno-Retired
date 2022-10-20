#pragma once

#include "constants.h"
#include "shader.h"
#include <glad/glad.h>
#include <stddef.h>
#include <vector>

#define EMPTY_BUFFER VertexBuffer(0, 0, 0)
#define EMPTY_UNIFORM_BUFFER 0

typedef GLuint VBO, VAO, EBO, UBO, IBO;
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

// Uniform buffer object.
struct JUniformBuffer
{
    UBO uniformBuffer;
    int index;
    static int currIndex;

    JUniformBuffer(size_t size, GLenum type);
    void ConnectToShader(JShader& shader, std::string blockName);
    void Bind();
    void SendData(size_t offset, void* data, size_t size);
    void Unbind();
    ~JUniformBuffer();
};

// Instance buffer object. Also can be used as a singular buffer.
struct JInstanceBuffer
{
    IBO instanceBuffer;

    JInstanceBuffer(void* data, size_t size, GLenum type);
    void Bind();
    ~JInstanceBuffer();
};