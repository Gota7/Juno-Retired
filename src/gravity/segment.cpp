#include "segment.h"

#include <tracy/Tracy.hpp>

RGravitySegment::RGravitySegment(glm::vec3 p1, glm::vec3 p2, glm::vec3 sideDir, float validSideDegree, bool edge1, bool edge2) : sideVec(glm::normalize(sideDir)), validSideDegree(validSideDegree)
{
    ZoneScopedN("RGravitySegment::RGravitySegment");

    points[0] = p1;
    points[1] = p2;
    edgeValid[0] = edge1;
    edgeValid[1] = edge2;
    UpdateParams();
    UpdateMtxIdentity();
}

void RGravitySegment::UpdateParams()
{
    ZoneScopedN("RGravitySegment::UpdateParams");

    float theta = glm::radians(validSideDegree * 0.5f);
    validSideCos = glm::cos(theta);
    glm::vec3 tmp = points[1] - points[0];
    tmp = glm::normalize(tmp);
    float m = glm::dot(sideVec, tmp); // This thing may be correct idk.
    sideVec = glm::vec4(sideVec + tmp * -m, 1.0f) * glm::rotate(glm::mat4(1.0f), theta, tmp);
}

void RGravitySegment::UpdateMtx(const glm::mat4& mtx)
{
    ZoneScopedN("RGravitySegment::UpdateMtx");

    for (int i = 0; i < 2; i++) pointsTranslated[i] = mtx * glm::vec4(points[i], 1.0f);
    sideVecTranslated = mtx * glm::vec4(sideVec, 0.0f);
    directionTranslated = pointsTranslated[1] - pointsTranslated[0];
    lengthTranslated = glm::length(directionTranslated);
    directionTranslated /= lengthTranslated;
}

bool RGravitySegment::CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist)
{
    ZoneScopedN("RGravitySegment::CalcOwnGravity");

    // Check valid side.
    glm::vec3 tmp = pos - pointsTranslated[0];
    glm::vec3 dir;
    float dot = glm::dot(tmp, directionTranslated);
    if (validSideCos > -1 && glm::dot(sideVecTranslated, sideVecTranslated) >= 0.0f)
    {
        dir = directionTranslated * dot;
        dir = tmp - dir;
        dir = glm::normalize(dir);
        if (glm::dot(dir, sideVecTranslated) < validSideCos) return false;
    }
    if (dot >= 0 && dot <= lengthTranslated)
    {
        dir = pointsTranslated[0] + directionTranslated * dot;
    }
    else if (dot >= 0)
    {
        if (!edgeValid[1]) return false;
        dir = pointsTranslated[1];
    }
    else
    {
        if (!edgeValid[0]) return false;
        dir = pointsTranslated[0];
    }
    dir -= pos;
    *outDist = glm::length(dir);
    if (!InRange(*outDist)) return false;
    *outDir = dir / *outDist;
    return true;

}

glm::vec3 RGravitySegment::RandomInRange()
{
    ZoneScopedN("RGravitySegment::RandomInRange");

    float lerpAlpha = JRandom::RandomInRange(0.0f, 1.0f);
    glm::vec3 mid = pointsTranslated[0] * (1 - lerpAlpha) + pointsTranslated[1] * lerpAlpha;
    return mid + glm::vec3(JRandom::RandomInRange(-range, range), JRandom::RandomInRange(-range, range), JRandom::RandomInRange(-range, range));
}