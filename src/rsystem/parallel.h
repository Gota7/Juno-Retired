#pragma once

#include "gravity.h"

// Range type.
enum RGravityParallelRangeTypeEnum
{
    PARALLEL_RANGE_SPHERE,
    PARALLEL_RANGE_BOX,
    PARALLEL_RANGE_CYLINDER
};

// Distance type.
enum RGravityParallelDistanceTypeEnum
{
    PARALLEL_DISTANCE_DEFAULT,
    PARALLEL_DISTANCE_X,
    PARALLEL_DISTANCE_Y,
    PARALLEL_DISTANCE_Z
};

// For flat planes that you can travel on.
struct RGravityParallel : RGravity
{
    RGravityParallelRangeTypeEnum rangeType;
    RGravityParallelDistanceTypeEnum distanceType;
    float baseDistance;
    float cylinderRadius;
    float cylinderHeight;
    glm::vec3 boxExtents; // Is generated.
    glm::vec3 pos;
    glm::vec3 direction;
    glm::mat4 boxMtx;
    glm::vec3 posTranslated; // Is generated.
    glm::vec3 directionTranslated; // Is generated.
    glm::mat4 boxMtxTranslated; // Is generated.

    // Make a spherical range.
    RGravityParallel(glm::vec3 pos, glm::vec3 direction, RGravityParallelDistanceTypeEnum distanceType, float baseDistance);

    // Make a box range.
    RGravityParallel(glm::vec3 pos, glm::vec3 direction, RGravityParallelDistanceTypeEnum distanceType, glm::mat4 boxMtx);

    // Make a cylindrical range.
    RGravityParallel(glm::vec3 pos, glm::vec3 direction, RGravityParallelDistanceTypeEnum distanceType, float cylinderRadius, float cylinderHeight);

    // Spherical range.
    bool IsInSphericalRange(const glm::vec3& pos, float* outDist);

    // Box range.
    bool IsInBoxRange(const glm::vec3& pos, float* outDist);

    // Cylindrical range.
    bool IsInCylindricalRange(const glm::vec3& pos, float* outDist);

    // Check if it's in range.
    bool IsInRange(const glm::vec3& pos, float* outDist);

    // Virtual functions.
    virtual void UpdateMtx(const glm::mat4& mtx);
    virtual bool CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist);
    virtual glm::vec3 RandomInRange();

};