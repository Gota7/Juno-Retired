#include "model.h"

constexpr float FLOOR_TO_WALL_THRESHOLD = glm::radians(67.976f);
constexpr float WALL_TO_CEILING_THRESHOLD = glm::radians(180.0f - 36.889f);

bool KModel::CalcPenetration(KModelTriangle& tri, const glm::mat4& colliderMat, const glm::vec3& pos, float radius, const glm::vec3& gravDir, glm::vec3* outPenetration, KPenetrationType* outType)
{

    // Transform sphere to collider coordinates.
    glm::vec3 newPos = glm::inverse(colliderMat) * glm::vec4(pos, 1.0f);

    // Get initial variables.
    // EDIT: I think it is best to not have references here as dereferencing for a lot of math is not a good idea.
    glm::vec3 p = vectors[tri.originPointIndex];
    glm::vec3 n = vectors[tri.normalVectorIndex];
    glm::vec3 d0 = vectors[tri.direction0VectorIndex];
    glm::vec3 d1 = vectors[tri.direction1VectorIndex];
    glm::vec3 d2 = vectors[tri.direction2VectorIndex];
    glm::vec3 v = newPos - p; // Vector from p to sphere's position.

    // Get projection of V in each direction. Dot product works since all directions are normal.
    float vD0 = glm::dot(v, d0);
    float vD1 = glm::dot(v, d1);
    float vD2 = glm::dot(v, d2) - tri.length; // L must be subracted since d2 is L away from P.

    // If any of these values are not less than R, this means the sphere can not be in range of the triangle in any of the directions.
    if (vD0 >= radius || vD1 >= radius || vD2 >= radius) return false;

    // Ok, we know the sphere is in the prism formed by the triangle, but is it in range of the normal?
    float vN = glm::dot(v, n);
    if (vN >= radius || vN < -0.005f) return false; // Out of range!

    // Get if floor, wall, or ceiling.
    glm::vec3 normalDir = colliderMat * glm::vec4(n, 0.0f); // Shouldn't need to normalize?
    glm::vec3 upDir = -gravDir;
    float angle = glm::acos(glm::dot(normalDir, upDir)); // Normally divide by ||normalDir|| * ||upDir|| but that should be one.
    if (angle >= FLOOR_TO_WALL_THRESHOLD)
    {
        if (angle >= WALL_TO_CEILING_THRESHOLD)
        {
            *outType = PENETRATION_CEILING;
        }
        else
        {
            *outType = PENETRATION_WALL;
        }
    }
    else
    {
        *outType = PENETRATION_FLOOR;
    }

    // Figure out to do face, edge, or vertex test. First the components are orded by size.
    std::vector<float> dots;
    dots.push_back(vD0);
    dots.push_back(vD1);
    dots.push_back(vD2);
    std::sort(dots.begin(), dots.end());

    // We can skip directly to pass face test if all components are <= 0. More work needs to be done if this is not the case.
    if (vD0 > 0 && vD1 > 0 && vD2 > 0)
    {
        bool edgeTest = dots[0] * dots[1] > 0; // TODO!!!

        // Do the edge test.
        if (edgeTest)
        {
            float distEdgeToSphere = glm::sqrt(0.0f);
        }

        // Do the vertex test.
        else
        {
        }
    }

    // Face test was used, p = r - v * n.
    // *outPenetration = radius
    return true;

}