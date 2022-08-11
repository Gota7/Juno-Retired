#pragma once

#include "gravity.h"

// Point based gravity. Used for any spherical surfaces.
struct RGravityPoint : RGravity
{
    glm::vec3 pos; // Position of the point.
    glm::vec3 posTranslated; // Point position in world space.

    // Virtual functions.
    virtual void UpdateMtx(const glm::mat4& mtx);
    virtual bool CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist);

};