#include "wire.h"

#include <tracy/Tracy.hpp>

RGravityWire::RGravityWire(std::vector<glm::vec3> points) : points(points)
{
    ZoneScopedN("RGravityWire::RGravityWire");
    UpdateMtxIdentity();
}

void RGravityWire::UpdateMtx(const glm::mat4& mtx)
{
    ZoneScopedN("RGravityWire::UpdateMtx");

    pointsTranslated.clear();
    for (auto& p : points)
    {
        pointsTranslated.push_back(mtx * glm::vec4(p, 1.0));
    }
}

bool RGravityWire::CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist)
{
    ZoneScopedN("RGravityWire::CalcOwnGravity");

    // Sanity check.
    if (pointsTranslated.size() == 0) return false;

    // Get closest point.
    float bestSquared = INFINITY;
    glm::vec3 closestPoint;
    for (int i = 0; i < pointsTranslated.size() - 1; i++)
    {
        glm::vec3 tmp = pointsTranslated[i + 1] - pointsTranslated[i];
        float proj = glm::dot(tmp, pos) - glm::dot(tmp, pointsTranslated[i]);
        float coord = proj / glm::dot(tmp, tmp);
        glm::vec3 projectedPoint = pointsTranslated[i] + tmp * coord;
        float squaredDist = glm::dot(projectedPoint, projectedPoint);
        if (squaredDist < bestSquared)
        {
            closestPoint = projectedPoint;
            bestSquared = squaredDist;
        }
    }

    // Return gravity.
    if (bestSquared == INFINITY || !InRangeSquared(bestSquared)) return false;
    *outDir = closestPoint - pos;
    *outDist = glm::length(*outDir);
    *outDir /= *outDist;
    return true;

}

glm::vec3 RGravityWire::RandomInRange()
{
    ZoneScopedN("RGravityWire::RandomInRange");

    float min = offset;
    float max = range;
    if (max < min) max = 1000.0f; // Some arbitrary high number.
    return pointsTranslated[(int)JRandom::RandomInRangeW(0, pointsTranslated.size())] + glm::normalize(glm::vec3(JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f))) * JRandom::RandomInRange(min, max);
}