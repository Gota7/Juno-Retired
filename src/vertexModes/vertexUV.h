#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

// Vertex positions.
struct VertexUV
{
    glm::vec3 vertex;
    glm::vec2 uv;

    VertexUV(glm::vec3 vertex, glm::vec2 uv) : vertex(vertex), uv(uv) {}

    static void SetAttributes()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)offsetof(VertexUV, vertex));
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexUV), (void*)offsetof(VertexUV, uv));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }
};