#include "buffers.h"
#include <tracy/Tracy.hpp>

VertexBuffer Buffers_Generate(void* vertexData, size_t vertexSize, GLenum vertexUsage, void* indexData, size_t indexSize, GLenum indexUsage)
{
    ZoneScopedN("Buffers_Generate");

    VBO vbo;
    VAO vao;
    EBO ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexSize, vertexData, vertexUsage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indexData, indexUsage);
    return VertexBuffer(vao, vbo, ebo);
}

void Buffers_Bind(VertexBuffer buffers)
{
    ZoneScopedN("Buffers_Bind");
    glBindVertexArray(buffers.vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.elementBuffer);
}

void Buffers_Delete(VertexBuffer buffers)
{
    ZoneScopedN("Buffers_Delete");
    glDeleteVertexArrays(1, &buffers.vertexArray);
    glDeleteBuffers(1, &buffers.vertexBuffer);
    glDeleteBuffers(1, &buffers.elementBuffer);
}

JBuffers::JBuffers(void* vertexData, size_t vertexSize, GLenum vertexUsage, void* indexData, size_t indexSize, GLenum indexUsage) : VertexBuffer(Buffers_Generate(vertexData, vertexSize, vertexUsage, indexData, indexSize, indexUsage))
{
    ZoneScopedN("JBuffers::JBuffers");
}

void JBuffers::Bind()
{
    ZoneScopedN("JBuffers::Bind");
    Buffers_Bind(*this);
}

JBuffers::~JBuffers()
{
    ZoneScopedN("JBuffers::~JBuffers");
    Buffers_Delete(*this);
}

int JUniformBuffer::currIndex = 0;

JUniformBuffer::JUniformBuffer(size_t size, GLenum type)
{
    ZoneScopedN("JUniformBuffer::JUniformBuffer");

    index = currIndex++;
    glGenBuffers(1, &uniformBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, type);
    glBindBuffer(GL_UNIFORM_BUFFER, EMPTY_UNIFORM_BUFFER);
    glBindBufferRange(GL_UNIFORM_BUFFER, index, uniformBuffer, 0, size);
}

void JUniformBuffer::ConnectToShader(JShader& shader, std::string blockName)
{
    ZoneScopedN("JUniformBuffer::ConnectToShader");

    GLuint index = glGetUniformBlockIndex(shader.shaderProgram, blockName.c_str());
    glUniformBlockBinding(shader.shaderProgram, index, this->index);
}

void JUniformBuffer::Bind()
{
    ZoneScopedN("JUniformBuffer::Bind");
    glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
}

void JUniformBuffer::SendData(size_t offset, void* data, size_t size)
{
    ZoneScopedN("JUniformBuffer::SendData");
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void JUniformBuffer::Unbind()
{
    ZoneScopedN("JUniformBuffer::Unbind");
    glBindBuffer(GL_UNIFORM_BUFFER, EMPTY_UNIFORM_BUFFER);
}

JUniformBuffer::~JUniformBuffer()
{
    ZoneScopedN("JUniformBuffer::~JUniformBuffer");
    glDeleteBuffers(1, &uniformBuffer);
    currIndex--;
}

JInstanceBuffer::JInstanceBuffer(void* data, size_t size, GLenum type)
{
    ZoneScopedN("JInstanceBuffer::JInstanceBuffer");
    glGenBuffers(1, &instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, size, data, type);
}

void JInstanceBuffer::Bind()
{
    ZoneScopedN("JInstanceBuffer::Bind");
    glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
}

JInstanceBuffer::~JInstanceBuffer()
{
    ZoneScopedN("JInstanceBuffer::~JInstanceBuffer");
    glDeleteBuffers(1, &instanceBuffer);
}