#pragma once

#include "gravity.h"

// Edge type.
enum RGravityTorusEdgeType
{
    TORUS_TYPE_NONE = 0,
    TORUS_TYPE_INSIDE = 0b1 << 0,
    TORUS_TYPE_OUTSIDE = 0b1 << 1,
    TORUS_TYPE_BOTH = TORUS_TYPE_INSIDE | TORUS_TYPE_OUTSIDE
};

// Torus/donut like gravity.
struct RGravityTorus : RGravity
{
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 posTranslated; // Set.
    glm::vec3 dirTranslated; // Set.
    float radius;
    float diskRadius;
    float radiusTranslated; // Set.
    RGravityTorusEdgeType edgeType;
    bool bothSides;

    // Make a new donut gravity shape.
    RGravityTorus(glm::vec3 pos, glm::vec3 dir, float radius, float diskRadius, RGravityTorusEdgeType edgeType = TORUS_TYPE_BOTH, bool bothSides = true);

    // Virtual functions.
    virtual void UpdateMtx(const glm::mat4& mtx);
    virtual bool CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist);
    virtual glm::vec3 RandomInRange();

};