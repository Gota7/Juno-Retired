#include "gravity.h"

#include <tracy/Tracy.hpp>

bool RGravity::CalcGravity(const glm::vec3& pos, glm::vec3* outDir, float* outMag)
{
    ZoneScopedN("RGravity::CalcGravity");

    glm::vec3 dir;
    float dist;
    if (!CalcOwnGravity(pos, &dir, &dist)) return false;

    // Adjust distance with offset.
    dist -= offset;

    // Galaxy does something weird here, but I'll just set magnitude to distance.
    if (outMag) *outMag = glm::abs(dist);

    // Invert gravity.
    if (inverted) dir = -dir;

    // Finished.
    if (outDir) *outDir = dir;
    return true;

}

bool RGravity::InRange(float radius)
{
    ZoneScopedN("RGravity::InRange");

    if (range < 0.0f)
    {
        return true;
    }
    else
    {
        float distance = offset + range; // Max gravity range.
        return radius < distance;
    }
}

bool RGravity::InRangeSquared(float radius)
{
    ZoneScopedN("RGravity::InRangeSquared");

    if (range < 0.0f)
    {
        return true;
    }
    else
    {
        float distance = offset + range; // Max gravity range.
        return radius < distance * distance;
    }
}

void RGravity::UpdateMtxIdentity()
{
    ZoneScopedN("RGravity::UpdateMtxIdentity");

    UpdateMtx(glm::mat4(1.0f));
}