#include "octree.h"
#include "util.h"

const unsigned int MAX_BASE_OCTREE_COUNT = 128;

KOctree::KOctree(std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec3>>& triangleData, unsigned int maxTriangles, float minWidth) : maxTriangles(maxTriangles), minWidth(minWidth)
{

    // Get triangles.
    for (unsigned int i = 0; i < triangleData.size(); i++)
    {
        triangles.push_back(i);
    }
    glm::vec3 min(0.0f);
    glm::vec3 max(0.0f);
    for (auto triangle : triangles)
    {
        min.x = glm::min(min.x, std::get<0>(triangleData[triangle]).x, std::get<1>(triangleData[triangle]).x, std::get<2>(triangleData[triangle]).x);
        max.x = glm::max(max.x, std::get<0>(triangleData[triangle]).x, std::get<1>(triangleData[triangle]).x, std::get<2>(triangleData[triangle]).x);
        min.y = glm::min(min.y, std::get<0>(triangleData[triangle]).y, std::get<1>(triangleData[triangle]).y, std::get<2>(triangleData[triangle]).y);
        max.y = glm::max(max.y, std::get<0>(triangleData[triangle]).y, std::get<1>(triangleData[triangle]).y, std::get<2>(triangleData[triangle]).y);
        min.z = glm::min(min.z, std::get<0>(triangleData[triangle]).z, std::get<1>(triangleData[triangle]).z, std::get<2>(triangleData[triangle]).z);
        max.z = glm::max(max.z, std::get<0>(triangleData[triangle]).z, std::get<1>(triangleData[triangle]).z, std::get<2>(triangleData[triangle]).z);
    }

    // Prevent 2d boxes.
    if (max.x == 0) max.x = glm::max(max.y, max.z);
    if (max.y == 0) max.x = glm::max(max.x, max.z);
    if (max.z == 0) max.x = glm::max(max.x, max.y);

    // Dimensions.
    width.x = glm::exp2(glm::ceil(glm::log2(glm::max(max.x - min.x, minWidth))));
    width.y = glm::exp2(glm::ceil(glm::log2(glm::max(max.y - min.y, minWidth))));
    width.z = glm::exp2(glm::ceil(glm::log2(glm::max(max.z - min.z, minWidth))));
    baseWidth = glm::min(width.x, width.y, width.z);
    this->base = min;

    // Cap boxes at limit.
    while (width.x * width.y * width.z / glm::pow(baseWidth, 3) > MAX_BASE_OCTREE_COUNT)
    {
        if (width.x == baseWidth) width.x *= 2;
        if (width.y == baseWidth) width.y *= 2;
        if (width.z == baseWidth) width.z *= 2;
        baseWidth *= 2;
    }
    nX = (int)glm::floor(width.x / baseWidth);
    nY = (int)glm::floor(width.y / baseWidth);
    nZ = (int)glm::floor(width.z / baseWidth);

    // Add children.
    for (int k = 0; k < nZ; k++)
    {
        for (int j = 0; j < nY; j++)
        {
            for (int i = 0; i < nX; i++)
            {
                children.push_back(std::make_unique<KOctree>((base + glm::vec3(i, j, k)) * baseWidth, baseWidth, triangles, triangleData, maxTriangles, minWidth));
            }
        }
    }

}

KOctree::KOctree(glm::vec3 base, float width, std::vector<unsigned int> triangles, std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec3>>& triangleData, unsigned int maxTriangles, float minWidth) : maxTriangles(maxTriangles), minWidth(minWidth)
{
    glm::vec3 center = base + glm::vec3(width, width, width) / 2.0f;
    this->width = glm::vec3(width, width, width) / 2.0f; // Original is just 0/2 which is 0, probably a bug.
    baseWidth = glm::min(this->width.x, this->width.y, this->width.z);
    this->base = base;
    for (auto& tri : triangles)
    {
        if (TriboxOverlap(triangleData[tri], center, width / 2.0f))
        {
            this->triangles.push_back(tri);
        }
    }
    isLeaf = true;
    if (this->triangles.size() > maxTriangles && width >= (2 * minWidth))
    {
        for (int k = 0; k < 2; k++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int i = 0; i < 2; i++)
                {
                    children.push_back(std::make_unique<KOctree>(base + glm::vec3(i, j, k) / 2.0f * width, width / 2.0f, this->triangles, triangleData, maxTriangles, minWidth));
                }
            }
        }
        triangles.clear();
        isLeaf = false;
    }
}

void KOctree::GetTriangles(const glm::vec3& pos, float radius, std::vector<unsigned int>& list)
{
    if (SphereboxOverlap(pos, radius, base, base + width))
    {
        if (isLeaf)
        {
            list.insert(list.end(), triangles.begin(), triangles.end());
        }
        else
        {
            for (auto& child : children)
            {
                child->GetTriangles(pos, radius, list);
            }
        }
    }
}

bool KOctree::SphereboxOverlap(const glm::vec3& pos, float radius, const glm::vec3& c1, const glm::vec3& c2)
{
    float distSquared = radius * radius;
    if (pos.x < c1.x) distSquared -= KUtil::Square(pos.x - c1.x);
    else if (pos.x > c2.x) distSquared -= KUtil::Square(pos.x - c2.x);
    if (pos.y < c1.y) distSquared -= KUtil::Square(pos.y - c1.y);
    else if (pos.y > c2.y) distSquared -= KUtil::Square(pos.y - c2.y);
    if (pos.z < c1.z) distSquared -= KUtil::Square(pos.z - c1.z);
    else if (pos.z > c2.z) distSquared -= KUtil::Square(pos.z - c2.z);
    return distSquared > 0;
}

bool KOctree::TriboxOverlap(std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec3>& tri, glm::vec3 center, float halfWidth)
{
    glm::vec3 u = std::get<0>(tri) - center;
    glm::vec3 v = std::get<1>(tri) - center;
    glm::vec3 w = std::get<2>(tri) - center;
    glm::vec3 n = std::get<3>(tri);
    if (glm::max(u.x, v.x, w.x) < -halfWidth || glm::min(u.x, v.x, w.x) > halfWidth) return false;
    if (glm::max(u.y, v.y, w.y) < -halfWidth || glm::min(u.y, v.y, w.y) > halfWidth) return false;
    if (glm::max(u.z, v.z, w.z) < -halfWidth || glm::min(u.z, v.z, w.z) > halfWidth) return false;
    float d = glm::dot(n, u);
    float r = halfWidth * (glm::abs(n.x) + glm::abs(n.y) + glm::abs(n.z));
    if (d < -r || d > r) return false;
    if (EdgeTest(u, v, w, halfWidth)) return false;
    if (EdgeTest(v, w, u, halfWidth)) return false;
    if (EdgeTest(w, u, v, halfWidth)) return false;
    return true;
}

bool KOctree::EdgeTest(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, float hw)
{
    glm::vec3 e = v1 - v0;
    if (EdgeAxisTest(e.z, -e.y, v0.y, v0.z, v2.y, v2.z, hw))
        return true;
    if (EdgeAxisTest(-e.z, e.x, v0.x, v0.z, v2.x, v2.z, hw))
        return true;
    if (EdgeAxisTest(e.y, -e.x, v0.x, v0.y, v2.x, v2.y, hw))
        return true;
    return false;
}

bool KOctree::EdgeAxisTest(float a1, float a2, float b1, float b2, float c1, float c2, float hw)
{
    float p = a1 * b1 + a2 * b2;
    float q = a1 * c1 + a2 * c2;
    float r = hw * (glm::abs(a1) + glm::abs(a2));
    return glm::max(p, q) < -r || glm::min(p, q) > r;
}