#pragma once

#include "gravity.h"

// Disk based gravity, a curved surface with two sides.
struct RGravityDisk
{
    glm::vec3 pos;
    glm::vec3 direction;
    glm::vec3 sideDirection;
    glm::vec3 sideDirectionOrtho;
    // TODO!!!
    glm::vec3 posTranslated;
    glm::vec3 directionTranslated;
    bool bothSides;

    // Virtual functions.
    virtual void UpdateMtx(const glm::mat4& mtx);
    virtual bool CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist);

};