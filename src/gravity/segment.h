#pragma once

#include "gravity.h"

// Segment gravity, for ramps and stuff.
struct RGravitySegment : RGravity
{
    glm::vec3 points[2];
    glm::vec3 sideVec;
    glm::vec3 pointsTranslated[2]; // Is generated.
    glm::vec3 sideVecTranslated; // Is generated.
    glm::vec3 directionTranslated; // Is generated.
    float lengthTranslated; // Is generated.
    float validSideDegree;
    float validSideCos; // Is generated.
    bool edgeValid[2];

    // Constructor.
    RGravitySegment(glm::vec3 p1, glm::vec3 p2, glm::vec3 sideDir, float validSideDegree = 360.0f, bool edge1 = true, bool edge2 = true);

    // Update parameters.
    void UpdateParams();

    // Virtual functions.
    virtual void UpdateMtx(const glm::mat4& mtx);
    virtual bool CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist);
    virtual glm::vec3 RandomInRange();

};