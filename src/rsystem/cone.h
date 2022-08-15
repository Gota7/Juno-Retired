#pragma once

#include "gravity.h"

// Cone gravity.
struct RGravityCone : RGravity
{
    glm::mat4 mtx;
    glm::mat4 mtxTranslated; // Is set.
    float topCutRate;
    float magTranslated; // Is set.
    bool enableBottom;

    // Virtual functions.
    virtual void UpdateMtx(const glm::mat4& mtx);
    virtual bool CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist);
    virtual glm::vec3 RandomInRange();

};