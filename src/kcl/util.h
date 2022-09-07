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

    // Square an item.
    static inline constexpr auto Square(auto val)
    {
        return val * val;
    }

    // Get an orthonormal vector.
    static glm::vec3 OrthoNormal(const glm::vec3& v);

    // Scale a float by a matrix.
    static inline float ScaleFloat(const glm::mat4& mat, float val)
    {
        return glm::length(mat[0]) * val; // Just multiply by size of the X axis.
    }

};