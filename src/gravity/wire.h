#pragma once

#include "gravity.h"
#include <vector>

// Wire gravity of multiple points.
struct RGravityWire : RGravity
{
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> pointsTranslated; // Set.

    // Construct a wire gravity line.
    RGravityWire(std::vector<glm::vec3> points);

    // Virtual functions.
    virtual void UpdateMtx(const glm::mat4& mtx);
    virtual bool CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist);
    virtual glm::vec3 RandomInRange();

};