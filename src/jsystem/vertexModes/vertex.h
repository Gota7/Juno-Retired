#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

// Vertex positions.
struct Vertex
{
    glm::vec3 vertex;

    Vertex(glm::vec3 vertex) : vertex(vertex) {}

    static void SetAttributes()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertex));
        glEnableVertexAttribArray(0);
    }
};