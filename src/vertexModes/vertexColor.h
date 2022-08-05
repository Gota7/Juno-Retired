#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

// Vertex color positions.
struct VertexColor
{
    glm::vec3 vertex;
    glm::vec3 color;

    VertexColor(glm::vec3 vertex, glm::vec3 color) : vertex(vertex), color(color) {}

    static void SetAttributes()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)sizeof(Vec3));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }
};