#pragma once

#include "gravity.h"

// Area flags.
enum RGravityCubeAreaFlags
{
    CUBE_AREA_BAD = -1,
    CUBE_AREA_X_LEFT = 0, CUBE_AREA_Y_LEFT = 0, CUBE_AREA_Z_LEFT = 0,
    CUBE_AREA_X_INSIDE = 1,
    CUBE_AREA_X_RIGHT = 2,
    CUBE_AREA_Y_INSIDE = 3,
    CUBE_AREA_Y_RIGHT = 6,
    CUBE_AREA_Z_INSIDE = 9,
    CUBE_AREA_Z_RIGHT = 18
};

// Valid area flags.
enum RGravityCubeValidAreaFlags
{
    CUBE_AREA_VALID_X_RIGHT = 0b1 << 0,
    CUBE_AREA_VALID_X_LEFT = 0b1 << 1,
    CUBE_AREA_VALID_Y_RIGHT = 0b1 << 2,
    CUBE_AREA_VALID_Y_LEFT = 0b1 << 3,
    CUBE_AREA_VALID_Z_RIGHT = 0b1 << 4,
    CUBE_AREA_VALID_Z_LEFT = 0b1 << 5,
    CUBE_AREA_VALID_ALL = CUBE_AREA_VALID_X_RIGHT | CUBE_AREA_VALID_X_LEFT | CUBE_AREA_VALID_Y_RIGHT | CUBE_AREA_VALID_Y_LEFT | CUBE_AREA_VALID_Z_RIGHT | CUBE_AREA_VALID_Z_LEFT
};

// Cube gravity.
struct RGravityCube : RGravity
{
    glm::mat4 mtx;
    glm::mat4 mtxTranslated; // Set.
    glm::vec3 extents; // Set.
    RGravityCubeValidAreaFlags validAreas;

    // Construct cube gravity.
    RGravityCube(glm::mat4 mtx, RGravityCubeValidAreaFlags validAreas = CUBE_AREA_VALID_ALL);

    // Get gravity area flags.
    int CalcGravityArea(const glm::vec3& pos);

    // Face gravity.
    bool CalcFaceGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist, int flags);

    // Edge gravity.
    bool CalcEdgeGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist, int flags);

    // Corner gravity.
    bool CalcCornerGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist, int flags);

    // Virtual functions.
    virtual void UpdateMtx(const glm::mat4& mtx);
    virtual bool CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist);
    virtual glm::vec3 RandomInRange();

};