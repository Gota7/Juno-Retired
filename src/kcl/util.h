#pragma once

#include "../jsystem.h"

// General collision utils.
struct KUtil
{

    // See if a point is in a boxed range.
    static bool PointInBox(const glm::vec3& point, const glm::vec3& pos, const glm::vec3& size);

    // Get combined box range.
    static void CombinedBoxRange(const glm::vec3& aPos, const glm::vec3& aSize, const glm::vec3& bPos, const glm::vec3& bSize, glm::vec3* outPos, glm::vec3* outSize);

    // Surface area of a box.
    static float SurfaceArea(const glm::vec3& size);

};