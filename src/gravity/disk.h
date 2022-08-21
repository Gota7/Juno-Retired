#pragma once

#include "gravity.h"

// Disk based gravity, a curved surface with two sides.
struct RGravityDisk : RGravity
{
    glm::vec3 pos;
    glm::vec3 direction;
    glm::vec3 sideDirection;
    glm::vec3 sideDirectionOrtho; // Is updated.
    glm::vec3 posTranslated; // Is set.
    glm::vec3 directionTranslated; // Is set.
    glm::vec3 sideDirectionTranslated; // Is set.
    float radius;
    float worldRadius; // Is set.
    float validDegrees;
    float validCos; // Is updated.
    bool bothSides;
    bool edgeGravity;

    // Constructor.
    RGravityDisk(glm::vec3 pos, glm::vec3 direction, glm::vec3 sideDirection, float radius, float validDegrees = 360.0f, bool bothSides = true, bool edgeGravity = true);

    // Update parameters after changing some.
    void UpdateParams();

    // Virtual functions.
    virtual void UpdateMtx(const glm::mat4& mtx);
    virtual bool CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist);
    virtual glm::vec3 RandomInRange();

};