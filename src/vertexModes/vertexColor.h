#pragma once

#include "../vec3.h"
#include <glad/glad.h>

// Vertex color positions.
struct VertexColor
{
    Vec3 vertex;
    Vec3 color;

    VertexColor(Vec3 vertex, Vec3 color) : vertex(vertex), color(color) {}

    static void SetAttributes()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)sizeof(Vec3));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }
};