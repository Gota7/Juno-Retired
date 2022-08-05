#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

// Vertex color positions.
struct VertexColorUV
{
    glm::vec3 vertex;
    glm::vec3 color;
    glm::vec2 uv;

    VertexColorUV(glm::vec3 vertex, glm::vec3 color, glm::vec2 uv) : vertex(vertex), color(color), uv(uv) {}

    static void SetAttributes()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColorUV), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColorUV), (void*)sizeof(glm::vec3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexColorUV), (void*)(sizeof(glm::vec3) * 2));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
};