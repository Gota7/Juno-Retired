#pragma once

#include "../vec2.h"
#include "../vec3.h"
#include <glad/glad.h>

// Vertex color positions.
struct VertexColorUV
{
    Vec3 vertex;
    Vec3 color;
    Vec2 uv;

    VertexColorUV(Vec3 vertex, Vec3 color, Vec2 uv) : vertex(vertex), color(color), uv(uv) {}

    static void SetAttributes()
    {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColorUV), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColorUV), (void*)sizeof(Vec3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexColorUV), (void*)(sizeof(Vec3) * 2));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
    }
};