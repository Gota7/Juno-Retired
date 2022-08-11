#include "disk.h"

void RGravityDisk::UpdateMtx(const glm::mat4& mtx)
{
    // TODO!!!
}

bool RGravityDisk::CalcOwnGravity(const glm::vec3& pos, glm::vec3* outDir, float* outDist)
{

    // Direction to position.
    glm::vec3 dirToPos = pos - posTranslated;
    float dot = glm::dot(dirToPos, directionTranslated);

    // Wrong side.
    if (dot < 0.0f && !bothSides) return false;
    return false; // TODO!!!

}