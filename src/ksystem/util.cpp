#include "util.h"

bool KUtil::PointInBox(const glm::vec3& point, const glm::vec3& pos, const glm::vec3& size)
{
    if (point.x < pos.x || point.x > pos.x + size.x) return false;
    if (point.y < pos.y || point.y > pos.y + size.y) return false;
    if (point.z < pos.z || point.z > pos.z + size.z) return false;
    return true;
}

void KUtil::CombinedBoxRange(const glm::vec3& aPos, const glm::vec3& aSize, const glm::vec3& bPos, const glm::vec3& bSize, glm::vec3* outPos, glm::vec3* outSize)
{
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
    return size.x * size.y * 2.0f + size.x * size.z * 2.0f + size.y * size.z * 2.0f;
}