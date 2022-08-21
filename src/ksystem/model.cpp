#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

constexpr float FLOOR_TO_WALL_THRESHOLD = glm::radians(67.976f); // TODO: CONVERT THESE TO NON-DOT PRODUCTS!
constexpr float WALL_TO_CEILING_THRESHOLD = glm::radians(180.0f - 36.889f);

KModel::KModel(std::string path, glm::mat4 matrix)
{

    // Initial setup.
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_OptimizeMeshes);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    // Import root node.
    ImportNode(scene, scene->mRootNode);

}

void KModel::ImportNode(const aiScene* scene, aiNode* node)
{

    // Import all meshes.
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ImportMesh(scene, mesh);
    }

    // Recursively search for other nodes.
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ImportNode(scene, node->mChildren[i]);
    }

}

void KModel::ImportMesh(const aiScene* scene, aiMesh* mesh)
{

    // TODO!!!

}

bool KModel::CalcPenetration(KModelTriangle& tri, const glm::vec3& pos, float radius, const glm::vec3& gravDir, KModelPenetrationInfo* outInfo)
{

    // Transform sphere to collider coordinates.
    glm::vec3 newPos = invMatrix * glm::vec4(pos, 1.0f);

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
    glm::vec3 normalDir = matrix * glm::vec4(n, 0.0f); // Shouldn't need to normalize?
    glm::vec3 upDir = -gravDir;
    float angle = glm::dot(normalDir, upDir); // Normally divide by ||normalDir|| * ||upDir|| but that should be one.
    if (angle >= FLOOR_TO_WALL_THRESHOLD)
    {
        if (angle >= WALL_TO_CEILING_THRESHOLD)
        {
            outInfo->type = PENETRATION_CEILING;
        }
        else
        {
            outInfo->type = PENETRATION_WALL;
        }
    }
    else
    {
        outInfo->type = PENETRATION_FLOOR;
    }

    // Figure out to do face, edge, or vertex test. First the components are orded by size.
    std::vector<float> dots;
    dots.push_back(vD0);
    dots.push_back(vD1);
    dots.push_back(vD2);
    std::sort(dots.begin(), dots.end());
    std::reverse(dots.begin(), dots.end());

    // Order directions appropriately. Probably not the most optimal but I really don't know how to do this.
    std::vector<glm::vec3&> dirs;
    if (dots[0] == vD0) dirs.push_back(d0);
    else if (dots[0] == vD1) dirs.push_back(d1);
    else if (dots[0] == vD2) dirs.push_back(d2);
    if (dots[1] == vD0) dirs.push_back(d0);
    else if (dots[1] == vD1) dirs.push_back(d1);
    else if (dots[1] == vD2) dirs.push_back(d2);
    if (dots[2] == vD0) dirs.push_back(d0);
    else if (dots[2] == vD1) dirs.push_back(d1);
    else if (dots[2] == vD2) dirs.push_back(d2);

    // We can skip directly to pass face test if all components are <= 0. More work needs to be done if this is not the case.
    if (vD0 > 0 && vD1 > 0 && vD2 > 0)
    {
        bool edgeTest = dots[0] * glm::dot(dirs[0], dirs[1]) > dots[1];
        float vh;

        // Do the edge test.
        if (edgeTest)
        {

            // The closest edge is the one with the biggest dot product with the direction. I mean what did you expect.
            float distEdgeToSphere = glm::sqrt(glm::pow(dots[0], 2.0f) + glm::pow(vN, 2.0f));
            if (distEdgeToSphere >= radius) return false; // Too far from the edge to collide.
            vh = dots[0]; // Wow so easy!

        }

        // Do the vertex test.
        else
        {
            float& a0 = dots[0];
            float& a1 = dots[1];
            float& aN = vN;
            float c = glm::dot(dirs[0], dirs[1]);
            if (glm::pow(a1 - a0 * c, 2.0f) / (1 - glm::pow(c, 2.0f)) + glm::pow(a0, 2.0f) + glm::pow(aN, 2.0f) < glm::pow(radius, 2.0f)) return false; // Formula is here: https://youtu.be/6hUK1Wbajt4?list=PL0TeYaSr_hNedZtktHufaFNO1usnQOuom&t=352
            vh = glm::sqrt(
                glm::pow(dots[1] - dots[0] * glm::dot(dirs[0], dirs[1]), 2.0f)
                / (1 - glm::pow(glm::dot(dirs[0], dirs[1]), 2.0f))
                    + glm::pow(dots[1], 2.0f)
            ); // What in the actual fuck.
        }
        outInfo->penetration = normalDir * (glm::sqrt(glm::pow(radius, 2.0f) - glm::pow(vh, 2.0f)) - vN);
        outInfo->faceCollision = false;
        return true;

    }

    // Face test was used, p = r - v * n.
    outInfo->penetration = normalDir * (radius - vN);
    outInfo->faceCollision = true;
    return true;

}

void KModel::Unpenetrate(glm::vec3& pos, std::vector<KModelPenetrationInfo> penetrations)
{

    // First get the aggregate vector.
    if (penetrations.size() == 0) return;
    glm::vec3 minBounds(0.0f);
    glm::vec3 maxBounds(0.0f); // We must cap at origin since we don't want a min to be positive and the other way around.
    for (auto& pen : penetrations)
    {
        glm::vec3 bounds(0.0f);
        switch (pen.type)
        {
            case PENETRATION_FLOOR:
                bounds += glm::vec3(0.0f, pen.penetration.y, 0.0f); // Vertical component only.
                break;
            case PENETRATION_WALL:
                bounds += glm::vec3(pen.penetration.x, 0.0f, pen.penetration.z); // Horizontal component only.
                break;
            case PENETRATION_CEILING:
                bounds += pen.penetration; // Use all for ceiling.
                break;
        }
        if (bounds.x < minBounds.x) minBounds.x = bounds.x;
        if (bounds.y < minBounds.y) minBounds.y = bounds.y;
        if (bounds.z < minBounds.z) minBounds.z = bounds.z;
        if (bounds.x > maxBounds.x) maxBounds.x = bounds.x;
        if (bounds.y > maxBounds.y) maxBounds.y = bounds.y;
        if (bounds.z > maxBounds.z) maxBounds.z = bounds.z;
        pos += minBounds + maxBounds; // Just add the bounds together to transform the sphere back. Sphere is already in world coordinates.
    }
}