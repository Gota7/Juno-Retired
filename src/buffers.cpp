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
}