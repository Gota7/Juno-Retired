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

    // Create a cone field.
    RGravityCone(glm::mat4 mtx, float topCutRate = 0.0f, bool enableBottom = false);

    // Project element.
    glm::vec3 ProjElem(glm::vec3& a, glm::vec3& b, float* outLen);

    // Project position onto line between two points.
    glm::vec3 ProjToLine(const glm::vec3& pos, glm::vec3& p0, glm::vec3& p1);

    // Calculate gravity from a mass position.
    bool CalcGravityFromPos(const glm::vec3& pos, glm::vec3 other, glm::vec3* outDir, float* outDist);

    // Lerp a vector.
    glm::vec3 LerpVec(glm::vec3& a, glm::vec3& b, float alpha);

    // Lerp a value.
    float LerpVal(float a, float b, float alpha);

    // Virtual functions.
    virtual void UpdateMtx(const glm::mat4& mtx);
    virtual bool CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist);
    virtual glm::vec3 RandomInRange();

};