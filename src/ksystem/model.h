#pragma once

#include "../jsystem.h"

// Model triangles.
// See https://www.youtube.com/watch?v=6hUK1Wbajt4&list=PL0TeYaSr_hNedZtktHufaFNO1usnQOuom&index=5
struct KModelTriangle
{
    float length;
    unsigned int originPointIndex; // Origin point.
    unsigned int normalVectorIndex; // Normal/up direction.
    unsigned int direction0VectorIndex; // Direction left of point.
    unsigned int direction1VectorIndex; // Direction right of point.
    unsigned int direction2VectorIndex; // Outward/forward direction.
    unsigned int terrainType; // Index of material in linked CLPS.
};

// Penetration type.
enum KPenetrationType
{
    PENETRATION_FLOOR,
    PENETRATION_WALL,
    PENETRATION_CEILING
};

// JKCL model file format.
struct KModel
{
    std::vector<glm::vec3> points; // Collection of origin points.
    std::vector<glm::vec3> vectors; // Direction vectors. ALL DIRECTIONS SHOULD BE NORMALIZED!
    std::vector<KModelTriangle> triangles; // Actual triangles that make up the model.
    // TODO: OCTREE!

    // Calculate penetration of a sphere relative to collision coordinates. Returns if it succeeds.
    bool CalcPenetration(KModelTriangle& tri, const glm::mat4& colliderMat, const glm::vec3& pos, float radius, const glm::vec3& gravDir, glm::vec3* outPenetration, KPenetrationType* outType);

};