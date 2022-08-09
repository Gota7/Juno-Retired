#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

// Vertex normal positions.
struct VertexNormalUV
{
    glm::vec3 vertex;
    glm::vec3 normal;
    glm::vec2 uv;

    VertexNormalUV(glm::vec3 vertex, glm::vec3 normal, glm::vec2 uv) : vertex(vertex), normal(normal), uv(uv) {}

    static void SetAttributes()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormalUV), (void*)offsetof(VertexNormalUV, vertex));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormalUV), (void*)offsetof(VertexNormalUV, normal));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexNormalUV), (void*)offsetof(VertexNormalUV, uv));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
};