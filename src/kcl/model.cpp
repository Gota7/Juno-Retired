#include "model.h"
#include "util.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

constexpr float FLOOR_TO_WALL_THRESHOLD = 0.375f; // Floors should generally be facing in the same direction as up but not too much.
constexpr float WALL_TO_CEILING_THRESHOLD = -0.8f; // Have to be similar in the opposite direction for ceiling.

KModel::KModel(std::string path, glm::mat4 matrix) : matrix(matrix)
{

    // Initial setup.
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    // Import root node.
    ImportNode(scene, scene->mRootNode);
    octree = KOctree(triangleData, 15, 1.0f);
    pointIndices.clear();
    vectorIndices.clear();
    triangleData.clear(); // These are only used for construction.
    invMatrix = glm::inverse(matrix);

}

unsigned int KModel::GetOrAddPoint(glm::vec3 pt)
{
    size_t hash = ((std::hash<float>{}(pt.x) * 31 + std::hash<float>{}(pt.y)) * 31 + std::hash<float>{}(pt.z)) * 31;
    if (pointIndices.find(hash) == pointIndices.end())
    {
        pointIndices[hash] = points.size();
        points.push_back(pt);
    }
    return pointIndices[hash];
}

unsigned int KModel::GetOrAddVec(glm::vec3 vec)
{
    size_t hash = ((std::hash<float>{}(vec.x) * 31 + std::hash<float>{}(vec.y)) * 31 + std::hash<float>{}(vec.z)) * 31;
    if (vectorIndices.find(hash) == vectorIndices.end())
    {
        vectorIndices[hash] = vectors.size();
        vectors.push_back(vec);
    }
    return vectorIndices[hash];
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

    // Just iterate over triangles.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace* face = &mesh->mFaces[i]; // This is legal, we asked assimp to triangulate.
        glm::vec3 p1(mesh->mVertices[face->mIndices[0]].x, mesh->mVertices[face->mIndices[0]].y, mesh->mVertices[face->mIndices[0]].z);
        glm::vec3 p2(mesh->mVertices[face->mIndices[1]].x, mesh->mVertices[face->mIndices[1]].y, mesh->mVertices[face->mIndices[1]].z);
        glm::vec3 p3(mesh->mVertices[face->mIndices[2]].x, mesh->mVertices[face->mIndices[2]].y, mesh->mVertices[face->mIndices[2]].z);
        KModelTriangle tri;
        tri.terrainType = mesh->mMaterialIndex;
        tri.originPointIndex = GetOrAddPoint(p1);
        glm::vec3 v0 = p1 - p3;
        glm::vec3 v1 = p2 - p3;
        glm::vec3 n = glm::normalize(glm::cross(v0, v1));
        tri.normalVectorIndex = GetOrAddVec(n);
        glm::vec3 d0 = glm::normalize(glm::cross(p1 - p3, n));
        glm::vec3 d1 = glm::normalize(glm::cross(p2 - p1, n));
        glm::vec3 d2 = glm::normalize(glm::cross(p3 - p2, n));
        tri.direction0VectorIndex = GetOrAddVec(d0);
        tri.direction1VectorIndex = GetOrAddVec(d1);
        tri.direction2VectorIndex = GetOrAddVec(d2);
        tri.length = glm::dot(p2 - p1, d2);
        if (matToTriangleMeshes.find(mesh->mMaterialIndex) == matToTriangleMeshes.end())
        {
            matToTriangleMeshes[mesh->mMaterialIndex] = std::vector<unsigned int>();
        }
        matToTriangleMeshes[mesh->mMaterialIndex].push_back(triangles.size());
        numMaterials = glm::max(numMaterials, mesh->mMaterialIndex + 1);
        triangles.push_back(tri);
        triangleData.push_back({ p1, p2, p3, n });
    }

}

std::unique_ptr<JModel> KModel::ToJModel(JShader& shader)
{

    // Create all of the meshes. Doesn't have to be efficient, this is for debug purposes.
    std::vector<std::unique_ptr<JMesh>> meshes;
    std::vector<std::unique_ptr<JMaterial>> mats;
    for (unsigned int i = 0; i < numMaterials; i++)
    {
        auto& vecs = matToTriangleMeshes[i];
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        for (auto ind : vecs)
        {
            auto& tri = triangles[ind];
            glm::vec3 dir0 = vectors[tri.direction0VectorIndex];
            glm::vec3 dir1 = vectors[tri.direction1VectorIndex];
            glm::vec3 dir2 = vectors[tri.direction2VectorIndex];
            glm::vec3 n = vectors[tri.normalVectorIndex];
            glm::vec3 crossB = glm::cross(n, dir1);
            glm::vec3 crossA = glm::cross(n, dir0);
            float dotB = glm::dot(crossB, dir2);
            float dotA = glm::dot(crossA, dir2);
            glm::vec3 p1 = points[tri.originPointIndex];
            glm::vec3 p2 = p1 + crossB * (dotB != 0.0f ? tri.length / dotB : 0.0f);
            glm::vec3 p3 = p1 + crossA * (dotA != 0.0f ? tri.length / dotA : 0.0f);
            indices.push_back(vertices.size());
            vertices.push_back(Vertex(p1));
            indices.push_back(vertices.size());
            vertices.push_back(Vertex(p2));
            indices.push_back(vertices.size());
            vertices.push_back(Vertex(p3));
        }
        mats.push_back(std::make_unique<JMaterialSolid>());
        JMaterialSolid* mat = static_cast<JMaterialSolid*>(mats[mats.size() - 1].get());
        glm::vec3 color((255 - (39 * i)) % 255 / 255.0f, (255 - (49 * i)) % 255 / 255.0f, (41 * i) % 255 / 255.0f);
        mat->ambient = color;
        mat->diffuse = color;
        mat->specular = glm::vec3(0.0f);
        meshes.push_back(std::make_unique<JMesh>(
            &vertices[0],
            vertices.size() * sizeof(vertices[0]),
            GL_STATIC_DRAW,
            &indices[0],
            indices.size() * sizeof(indices[0]),
            GL_STATIC_DRAW,
            GL_TRIANGLES,
            indices.size(),
            GL_UNSIGNED_INT,
            i
        ));
        Vertex::SetAttributes();
    }
    return std::make_unique<JModel>(
        meshes,
        std::vector<std::string>(),
        mats,
        shader,
        matrix
    );

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
    if (angle >= WALL_TO_CEILING_THRESHOLD)
    {
        if (angle >= FLOOR_TO_WALL_THRESHOLD)
        {
            outInfo->type = PENETRATION_FLOOR;
        }
        else
        {
            outInfo->type = PENETRATION_WALL;
        }
    }
    else
    {
        outInfo->type = PENETRATION_CEILING;
    }

    // Figure out to do face, edge, or vertex test. First the components are orded by size.
    std::vector<float> dots;
    dots.push_back(vD0);
    dots.push_back(vD1);
    dots.push_back(vD2);
    std::sort(dots.begin(), dots.end());
    std::reverse(dots.begin(), dots.end());

    // Order directions appropriately. Probably not the most optimal but I really don't know how to do this.
    std::vector<glm::vec3> dirs;
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
            float distEdgeToSphere = glm::sqrt(KUtil::Square(dots[0]) + KUtil::Square(vN));
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
            if (KUtil::Square(a1 - a0 * c) / (1 - KUtil::Square(c)) + KUtil::Square(a0) + KUtil::Square(aN) < KUtil::Square(radius)) return false; // Formula is here: https://youtu.be/6hUK1Wbajt4?list=PL0TeYaSr_hNedZtktHufaFNO1usnQOuom&t=352
            vh = glm::sqrt(
                KUtil::Square(dots[1] - dots[0] * glm::dot(dirs[0], dirs[1]))
                / (1 - KUtil::Square(glm::dot(dirs[0], dirs[1])))
                    + KUtil::Square(dots[1])
            ); // What in the actual fuck.
        }
        outInfo->penetration = normalDir * (glm::sqrt(KUtil::Square(radius) - KUtil::Square(vh)) - vN);
        outInfo->faceCollision = false;
        return true;

    }

    // Face test was used, p = r - v * n.
    outInfo->penetration = normalDir * (radius - vN);
    outInfo->faceCollision = true;
    return true;

}

void KModel::Unpenetrate(glm::vec3& pos, std::vector<KModelPenetrationInfo>& penetrations)
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

glm::vec3 KModel::Position()
{
    return glm::vec3(matrix * glm::vec4(0.0f)); // Get new origin point.
}

glm::vec3 KModel::Range()
{
    return glm::vec3(1000.0f, 1000.0f, 1000.0f); // Idk very temporary.
}

void KModel::Uncollide(glm::vec3& pos, float radius, const glm::vec3& gravDir)
{

    // Search for triangles.
    std::vector<unsigned int> tris;
    octree.GetTriangles(pos, radius, tris);

    // Interact with the triangles.
    std::vector<KModelPenetrationInfo> pens;
    bool hasFloor = false;
    for (auto tri : tris)
    {
        KModelPenetrationInfo pen;
        if (CalcPenetration(triangles[tri], pos, radius, gravDir, &pen))
        {
            if (pen.type == PENETRATION_FLOOR)
            {
                if (!hasFloor) pens.push_back(pen); // We should only allow one floor collision.
                hasFloor = true;
            }
            else
            {
                pens.push_back(pen);
            }
        }
    }
    Unpenetrate(pos, pens); // Actually take care of making them not penetrated.

}