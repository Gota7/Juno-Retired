#include "parallel.h"

RGravityParallel::RGravityParallel(glm::vec3 pos, glm::vec3 direction, float baseDistance) : rangeType(PARALLEL_RANGE_SPHERE), pos(pos), direction(direction), baseDistance(baseDistance)
{
    UpdateMtxIdentity();
}

RGravityParallel::RGravityParallel(glm::vec3 pos, glm::vec3 direction, RGravityParallelDistanceTypeEnum distanceType, glm::mat4 boxMtx) : rangeType(PARALLEL_RANGE_BOX), pos(pos), direction(direction), distanceType(distanceType), boxMtx(boxMtx)
{
    UpdateMtxIdentity();
}

RGravityParallel::RGravityParallel(glm::vec3 pos, glm::vec3 direction, float cylinderRadius, float cylinderHeight) : rangeType(PARALLEL_RANGE_CYLINDER), pos(pos), direction(direction), cylinderRadius(cylinderRadius), cylinderHeight(cylinderHeight)
{
    UpdateMtxIdentity();
}

bool RGravityParallel::IsInSphericalRange(const glm::vec3& pos, float* outDist)
{
    if (range >= 0)
    {
        glm::vec3 diff = pos - posTranslated;
        if (glm::dot(diff, diff) < range * range)
        {
            *outDist = baseDistance;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        *outDist = baseDistance;
        return true;
    }
}

bool RGravityParallel::IsInBoxRange(const glm::vec3& pos, float* outDist)
{

    // Get local space coordinates.
    glm::vec3 localSpace = pos - glm::vec3(boxMtxTranslated[3]);

    // X axis.
    glm::vec3 axisX(boxMtxTranslated[0][0], boxMtxTranslated[0][1], boxMtxTranslated[0][2]);
    float dotX = glm::dot(localSpace, axisX);
    if (dotX < -boxExtents.x || dotX > boxExtents.x) return false;

    // Y axis.
    glm::vec3 axisY(boxMtxTranslated[1][0], boxMtxTranslated[1][1], boxMtxTranslated[1][2]);
    float dotY = glm::dot(localSpace, axisY);
    if (dotY < -boxExtents.y || dotY > boxExtents.y) return false;

    // Z axis.
    glm::vec3 axisZ(boxMtxTranslated[2][0], boxMtxTranslated[2][1], boxMtxTranslated[2][2]);
    float dotZ = glm::dot(localSpace, axisZ);
    if (dotZ < -boxExtents.z || dotZ > boxExtents.z) return false;

    if (distanceType == PARALLEL_DISTANCE_DEFAULT)
    {
        *outDist = baseDistance;
        return true;
    }
    else if (distanceType == PARALLEL_DISTANCE_X)
    {
        *outDist = baseDistance + (fabs(dotX) / glm::sqrt(boxExtents.x));
        return true;
    }
    else if (distanceType == PARALLEL_DISTANCE_Y)
    {
        *outDist = baseDistance + (fabs(dotY) / glm::sqrt(boxExtents.y));
        return true;
    }
    else if (distanceType == PARALLEL_DISTANCE_Z)
    {
        *outDist = baseDistance + (fabs(dotZ) / glm::sqrt(boxExtents.z));
        return true;
    }
    else
    {
        return false;
    }

}

bool RGravityParallel::IsInCylindricalRange(const glm::vec3& pos, float* outDist)
{
    glm::vec3 dirToPoint = pos - posTranslated; // Direction to point in field.
    float depth = glm::dot(dirToPoint, direction);
    dirToPoint = direction * -depth + dirToPoint;
    if (depth < 0.0f || depth > cylinderHeight) return false;
    float mag = glm::length(dirToPoint);
    if (mag > cylinderRadius) return false;
    *outDist = baseDistance + mag;
    return true;
}

bool RGravityParallel::IsInRange(const glm::vec3& pos, float* outDist)
{
    switch (rangeType)
    {
        case PARALLEL_RANGE_SPHERE: return IsInSphericalRange(pos, outDist);
        case PARALLEL_RANGE_BOX: return IsInBoxRange(pos, outDist);
        case PARALLEL_RANGE_CYLINDER: return IsInCylindricalRange(pos, outDist);
    }
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