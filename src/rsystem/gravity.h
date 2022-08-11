#pragma once

#include "../jsystem.h"
#include <glm/glm.hpp>

// Gravity types.
enum RGravityType
{
    GRAVITY_NORMAL = 0b1 << 0,
    GRAVITY_SHADOW = 0b1 << 1,
    GRAVITY_NORMAL_SHADOW = GRAVITY_NORMAL | GRAVITY_SHADOW
};

// Standard gravity template.
struct RGravity
{
    float offset = 0.0f; // Gravity offset from origin.
    float range = -1.0f; // Range of field from offset.
    float strength = 1.0f; // Strength of field.
    unsigned int priority = 50; // Higher priority means it will take over if another field is present.
    RGravityType type = GRAVITY_NORMAL_SHADOW; // What the gravity field effects.
    bool active = true; // Gravity field will only work if active.
    bool inverted = false; // If to invert the gravity.

    // Update internal position to be in world space.
    virtual void UpdateMtx(const glm::mat4& mtx) {}

    // Calculate the gravity direction vector for actor in range. Returns true for success.
    virtual bool CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist)
    {
        return false;
    }

    // Calculate the gravity direction. True only if gravity applies.
    bool CalcGravity(const glm::vec3& pos, glm::vec3* outDir, float* outMag);

    // If the given radius is in a spherical range of the gravity field.
    bool InRange(float radius);

    // If the given radius is in a squared spherical range of the gravity field.
    bool InRangeSquared(float radius);

    // Make it so the world space of the gravity field is relative to its own space.
    void UpdateMtxIdentity();

    // Render the gravity.
    void Render(JShader& shader);

};