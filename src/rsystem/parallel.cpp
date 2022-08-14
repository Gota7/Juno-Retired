#include "parallel.h"

RGravityParallel::RGravityParallel(glm::vec3 pos, glm::vec3 direction, RGravityParallelDistanceTypeEnum distanceType, float baseDistance) : rangeType(PARALLEL_RANGE_SPHERE), pos(pos), direction(direction), distanceType(distanceType), baseDistance(baseDistance)
{
    UpdateMtxIdentity();
}

RGravityParallel::RGravityParallel(glm::vec3 pos, glm::vec3 direction, RGravityParallelDistanceTypeEnum distanceType, glm::mat4 boxMtx) : rangeType(PARALLEL_RANGE_BOX), pos(pos), direction(direction), distanceType(distanceType), boxMtx(boxMtx)
{
    UpdateMtxIdentity();
}

RGravityParallel::RGravityParallel(glm::vec3 pos, glm::vec3 direction, RGravityParallelDistanceTypeEnum distanceType, float cylinderRadius, float cylinderHeight) : rangeType(PARALLEL_RANGE_CYLINDER), pos(pos), direction(direction), distanceType(distanceType), cylinderRadius(cylinderRadius), cylinderHeight(cylinderHeight)
{
    UpdateMtxIdentity();
}

bool RGravityParallel::IsInSphericalRange(const glm::vec3& pos, float* outDist)
{
    return false;
}

bool RGravityParallel::IsInBoxRange(const glm::vec3& pos, float* outDist)
{
    return false;
}

bool RGravityParallel::IsInCylindricalRange(const glm::vec3& pos, float* outDist)
{
    return false;
}

bool RGravityParallel::IsInRange(const glm::vec3& pos, float* outDist)
{
    return false;
}

void RGravityParallel::UpdateMtx(const glm::mat4& mtx)
{
    posTranslated = mtx * glm::vec4(pos, 1.0f);
    direction = glm::normalize(direction);
    directionTranslated = mtx * glm::vec4(direction, 0.0f);
    if (rangeType == PARALLEL_RANGE_BOX)
    {
        boxMtxTranslated = mtx * boxMtx;
        glm::vec3 axisX(boxMtxTranslated[0][0], boxMtxTranslated[0][1], boxMtxTranslated[0][2]);
        glm::vec3 axisY(boxMtxTranslated[1][0], boxMtxTranslated[1][1], boxMtxTranslated[1][2]);
        glm::vec3 axisZ(boxMtxTranslated[2][0], boxMtxTranslated[2][1], boxMtxTranslated[2][2]);
        boxExtents = glm::vec3(glm::dot(axisX, axisX), glm::dot(axisY, axisY), glm::dot(axisZ, axisZ));
    }
}

bool RGravityParallel::CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist)
{
    if (!IsInRange(pos, outDist)) return false;
    *outDir = -directionTranslated;
    return true;
}

glm::vec3 RGravityParallel::RandomInRange()
{
    float min = offset;
    float max = range;
    if (max < min) max = 1000.0f; // Some arbitrary high number.
    return posTranslated + glm::normalize(glm::vec3(rand() % 1000 - 500, rand() % 1000 - 500, rand() % 1000 - 500)) * ((rand() % 1000) / 1000.0f * (max - min) + min);
}