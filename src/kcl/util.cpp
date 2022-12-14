#include "util.h"

#include <tracy/Tracy.hpp>

bool KUtil::PointInBox(const glm::vec3& point, const glm::vec3& pos, const glm::vec3& size)
{
    ZoneScopedN("KUtil::PointInBox");

    if (point.x < pos.x || point.x > pos.x + size.x) return false;
    if (point.y < pos.y || point.y > pos.y + size.y) return false;
    if (point.z < pos.z || point.z > pos.z + size.z) return false;
    return true;
}

void KUtil::CombinedBoxRange(const glm::vec3& aPos, const glm::vec3& aSize, const glm::vec3& bPos, const glm::vec3& bSize, glm::vec3* outPos, glm::vec3* outSize)
{
    ZoneScopedN("KUtil::CombinedBoxRange");

    *outPos = glm::vec3(
        glm::min(aPos.x, bPos.x),
        glm::min(aPos.y, bPos.y),
        glm::min(aPos.z, bPos.z)
    );
    *outSize = glm::vec3(
        glm::max(aPos.x + aSize.x, bPos.x + bSize.x),
        glm::max(aPos.y + aSize.y, bPos.y + bSize.y),
        glm::max(aPos.z + aSize.z, bPos.z + bSize.z)
    ) - *outPos;
}

float KUtil::SurfaceArea(const glm::vec3& size)
{
    ZoneScopedN("KUtil::SurfaceArea");

    return size.x * size.y * 2.0f + size.x * size.z * 2.0f + size.y * size.z * 2.0f;
}

// Algorithm by pants64ds.
glm::vec3 KUtil::OrthoNormal(const glm::vec3& v)
{
    ZoneScopedN("KUtil::OrthoNormal");

    float xx = KUtil::Square(v.x);
    float yy = KUtil::Square(v.y);
    float zz = KUtil::Square(v.z);
    if (xx < yy)
    {
        if (xx < zz)
        {
            return glm::normalize(glm::vec3(0.0f, v.z, -v.y));
        }
    }
    else if (yy < zz)
    {
        return glm::normalize(glm::vec3(-v.z, 0.0f, v.x));
    }
    return glm::normalize(glm::vec3(v.y, -v.x, 0.0f));
}