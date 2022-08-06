#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

// Vertex normal positions.
struct VertexNormal
{
    glm::vec3 vertex;
    glm::vec3 normal;

    VertexNormal(glm::vec3 vertex, glm::vec3 normal) : vertex(vertex), normal(normal) {}

    static void SetAttributes()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), (void*)offsetof(VertexNormal, vertex));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexNormal), (void*)offsetof(VertexNormal, normal));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }
};