#include "point.h"

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
    if (outDir) *outDir = dir;
    if (outDist) *outDist = mag;
    return true;

}