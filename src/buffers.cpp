#include "buffers.h"

VertexBuffer Buffers_Generate(void* vertexData, size_t vertexSize, GLenum vertexUsage, void* indexData, size_t indexSize, GLenum indexUsage)
{
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
    glBindVertexArray(buffers.vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, buffers.vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.elementBuffer);
}

void Buffers_Delete(VertexBuffer buffers)
{
    glDeleteVertexArrays(1, &buffers.vertexArray);
    glDeleteBuffers(1, &buffers.vertexBuffer);
    glDeleteBuffers(1, &buffers.elementBuffer);
}

JBuffers::JBuffers(void* vertexData, size_t vertexSize, GLenum vertexUsage, void* indexData, size_t indexSize, GLenum indexUsage) : VertexBuffer(Buffers_Generate(vertexData, vertexSize, vertexUsage, indexData, indexSize, indexUsage))
{
}

void JBuffers::Bind()
{
    Buffers_Bind(*this);
}

JBuffers::~JBuffers()
{
    Buffers_Delete(*this);
}

int JUniformBuffer::currIndex = 0;

JUniformBuffer::JUniformBuffer(size_t size, GLenum type)
{
    index = currIndex++;
    glGenBuffers(1, &uniformBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, type);
    glBindBuffer(GL_UNIFORM_BUFFER, EMPTY_UNIFORM_BUFFER);
    glBindBufferRange(GL_UNIFORM_BUFFER, index, uniformBuffer, 0, size);
}

void JUniformBuffer::ConnectToShader(JShader& shader, std::string blockName)
{
    GLuint index = glGetUniformBlockIndex(shader.shaderProgram, blockName.c_str());
    glUniformBlockBinding(shader.shaderProgram, index, this->index);
}

void JUniformBuffer::Bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
}

void JUniformBuffer::SendData(size_t offset, size_t size, void* data)
{
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void JUniformBuffer::Unbind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, EMPTY_UNIFORM_BUFFER);
}

JUniformBuffer::~JUniformBuffer()
{
    glDeleteBuffers(1, &uniformBuffer);
    currIndex--;
}