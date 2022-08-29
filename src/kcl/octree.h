#pragma once

#include "../jsystem.h"

// Octree for organizing KModelTriangles.
struct KOctree
{
    std::vector<unsigned int> triangles;
    unsigned int maxTriangles;
    float minWidth;
    bool isLeaf = false;
    std::vector<std::unique_ptr<KOctree>> children;
    glm::vec3 width;
    float baseWidth;
    glm::vec3 base;
    int nX, nY, nZ; // Children count.

    // No initialization.
    KOctree() {}

    // Create a basic octree.
    KOctree(std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec3>>& triangleData, unsigned int maxTriangles, float minWidth);

    // Constructor for a child.
    KOctree(glm::vec3 base, float width, std::vector<unsigned int> triangles, std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec3>>& triangleData, unsigned int maxTriangles, float minWidth);

    // Gather triangles to collide with.
    void GetTriangles(const glm::vec3& pos, float radius, std::vector<unsigned int>& list);

    // If a sphere overlaps a cube.
    static bool SphereboxOverlap(const glm::vec3& pos, float radius, const glm::vec3& c1, const glm::vec3& c2);

    // If a triangle overlaps a cube.
    static bool TriboxOverlap(std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec3>& tri, glm::vec3 center, float halfWidth);

    // Test for edges of cubes.
    static bool EdgeTest(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, float hw);

    // Test for an edge by an axis.
    static bool EdgeAxisTest(float a1, float a2, float b1, float b2, float c1, float c2, float hw);

};