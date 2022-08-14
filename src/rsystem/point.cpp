#include "point.h"

RGravityPoint::RGravityPoint(glm::vec3 pos) : pos(pos)
{
    UpdateMtxIdentity();
}

void RGravityPoint::UpdateMtx(const glm::mat4& mtx)
{
    posTranslated = mtx * glm::vec4(pos, 1.0);
}

bool RGravityPoint::CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist)
{

    // Direction to point gravity.
    glm::vec3 dir = posTranslated - pos;
    float mag = glm::length(dir);
    if (mag < 0.01f)
    {
        dir = glm::vec3(0);
    }

    // Range check.
    if (!InRange(mag)) return false;

    // Update vectors.
    *outDir = dir;
    *outDist = mag;
    return true;

}

glm::vec3 RGravityPoint::RandomInRange()
{
    float min = offset;
    float max = range;
    if (max < min) max = 1000.0f; // Some arbitrary high number.
    return posTranslated + glm::normalize(glm::vec3(JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f))) * JRandom::RandomInRange(min, max);
}