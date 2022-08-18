#pragma once

#include "gravity.h"

// Gravity out info.
struct RGravityOutInfo
{
    glm::vec3 gravityDirection;
    unsigned int largestPriority;
    RGravity* gravity;
};

// Manager for gravity fields.
struct RManager
{
    std::vector<std::unique_ptr<RGravity>> gravities;

    // Add a gravity to the list. Uses existing priority by default.
    void AddGravity(std::unique_ptr<RGravity> gravity, unsigned int priority = 0);

    // Calculate gravity. Also can optionally store info about the biggest field.
    bool CalcGravity(const glm::vec3& pos, RGravityType type, glm::vec3* outDir, float* outMag = nullptr, RGravityOutInfo* outInfo = nullptr);

};