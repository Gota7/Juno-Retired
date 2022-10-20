#include "model.h"
#include "util.h"
#include "../fs.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <tracy/Tracy.hpp>

KModel::KModel(std::string path, glm::mat4 matrix, float triangleThickness) : matrix(matrix), triangleThickness(triangleThickness)
{
    ZoneScopedN("KModel::KModel");

    // Initial setup.
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(FPath::RelPath(path), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    // Import root node.
    ImportNode(scene, scene->mRootNode);
    octree = KOctree(triangleData, 15, 0.1f);
    pointIndices.clear();
    vectorIndices.clear();
    triangleData.clear(); // These are only used for construction.
    invMatrix = glm::inverse(matrix);

}

unsigned int KModel::GetOrAddPoint(glm::vec3 pt)
{
    ZoneScopedN("KModel::GetOrAddPoint");

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
    ZoneScopedN("KModel::GetOrAddVec");

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
    ZoneScopedN("KModel::ImportNode");

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
    ZoneScopedN("KModel::ImportMesh");

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

glm::vec3 KModel::GetVertex(KModelTriangle& tri, int vertexNum)
{
    ZoneScopedN("KModel::GetVertex");

    if (vertexNum == 1)
    {
        glm::vec3 a = IndexToVector(tri.direction1VectorIndex);
        glm::vec3 b = IndexToVector(tri.normalVectorIndex);
        a = glm::cross(a, b);
        b = IndexToVector(tri.direction2VectorIndex);
        float dist = tri.length / glm::dot(a, b);
        return IndexToPoint(tri.originPointIndex) + a * dist;
    }
    else if (vertexNum == 2)
    {
        glm::vec3 a = IndexToVector(tri.normalVectorIndex);
        glm::vec3 b = IndexToVector(tri.direction0VectorIndex);
        a = glm::cross(a, b);
        b = IndexToVector(tri.direction2VectorIndex);
        float dist = tri.length / glm::dot(a, b);
        return IndexToPoint(tri.originPointIndex) + a * dist;
    }
    else
    {
        return IndexToPoint(tri.originPointIndex);
    }
}

std::unique_ptr<JModel> KModel::ToJModel(JShader& shader)
{
    ZoneScopedN("KModel::ToJModel");

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
            indices.push_back(vertices.size());
            vertices.push_back(Vertex(GetVertex(tri, 0)));
            indices.push_back(vertices.size());
            vertices.push_back(Vertex(GetVertex(tri, 1)));
            indices.push_back(vertices.size());
            vertices.push_back(Vertex(GetVertex(tri, 2)));
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

// See https://github.com/magcius/noclip.website/blob/9cf0b026b4c00660cbcf3523bd9c3f9a078aa5a6/src/SuperMarioGalaxy/KCollisionServer.ts
bool KModel::CalcPenetrationArrow(KModelTriangle& tri, const glm::vec3& pos, const glm::vec3& dir, KModelPenetrationInfo& outInfo)
{
    ZoneScopedN("KModel::CalcPenetrationArrow");

    // Load variables.
    glm::vec3 p = IndexToPoint(tri.originPointIndex);
    glm::vec3 n = IndexToVector(tri.normalVectorIndex);
    glm::vec3 localPos = pos - p;

    // Projection.
    float proj = glm::dot(n, localPos);
    if (proj < 0.0f) return false;
    float projDir = glm::dot(n, dir);
    if (proj + projDir >= 0.0f) return false;

    // Distances.
    float dist = -(proj / projDir);
    glm::vec3 maxPos = localPos + dir * dist;

    // Try desting distance with each edge.
    glm::vec3 d0 = IndexToVector(tri.direction0VectorIndex);
    float d0Dot = glm::dot(maxPos, d0);
    if (d0Dot >= 0.01f) return false;

    glm::vec3 d1 = IndexToVector(tri.direction1VectorIndex);
    float d1Dot = glm::dot(maxPos, d1);
    if (d1Dot >= 0.01f) return false;

    glm::vec3 d2 = IndexToVector(tri.direction2VectorIndex);
    float d2Dot = glm::dot(maxPos, d2) - tri.length;
    if (d2Dot >= 0.01f) return false;

    // Return distance info.
    outInfo.collisionType = COLLISION_FACE;
    outInfo.distance = dist;
    return true;

}

bool KModel::CalcPenetrationSphere(KModelTriangle& tri, const glm::vec3& pos, float radius, float scale, KModelPenetrationInfo& outInfo)
{
    ZoneScopedN("KModel::CalcPenetration");

    // Get initial variables.
    // EDIT: I think it is best to not have references here as dereferencing for a lot of math is not a good idea.
    glm::vec3 p = points[tri.originPointIndex];
    glm::vec3 n = vectors[tri.normalVectorIndex];
    glm::vec3 d0 = vectors[tri.direction0VectorIndex];
    glm::vec3 d1 = vectors[tri.direction1VectorIndex];
    glm::vec3 d2 = vectors[tri.direction2VectorIndex];
    glm::vec3 v = pos - p; // Vector from p to sphere's position.

    // Get projection of V in each direction. Dot product works since all directions are normal.
    float vD0 = glm::dot(v, d0);
    if (vD0 >= radius) return false;
    float vD1 = glm::dot(v, d1);
    if (vD1 >= radius) return false;
    float vD2 = glm::dot(v, d2) - tri.length; // L must be subracted since d2 is L away from P.
    if (vD2 >= radius) return false;

    // Ok, we know the sphere is in the prism formed by the triangle, but is it in range of the normal?
    float vN = glm::dot(v, n);
    if (radius - vN < 0.0f) return false; // Out of range!

    // We can skip directly to pass face test if all components are <= 0. More work needs to be done if this is not the case.
    float dist;
    outInfo.collisionType = COLLISION_UNDEF;
    if (vD0 > 0 || vD1 > 0 || vD2 > 0)
    {

        // Declare vars.
        float radiusSq = KUtil::Square(radius);
        float t1;
        glm::vec3 edge1;
        glm::vec3 edge2;

        // vD0 is max.
        if (vD0 >= vD1 && vD0 >= vD2)
        {
            if (vD1 >= vD2) // vD1 2nd.
            {
                // Edges are 0 and 1.
                edge1 = d0;
                edge2 = d1;
                t1 = glm::dot(edge1, edge2);
                if (vD1 >= t1 * vD0) outInfo.collisionType = COLLISION_VERTEX0;
            }
            else // vD2 2nd.
            {
                // Edges are 2 and 0.
                edge1 = d2;
                edge2 = d0;
                t1 = glm::dot(edge1, edge2);
                if (vD2 >= t1 * vD0) outInfo.collisionType = COLLISION_VERTEX2;
            }
            if (outInfo.collisionType == COLLISION_UNDEF)
            {
                //if (vD0 > vN) return false;
                outInfo.collisionType = COLLISION_EDGE0;
                dist = radiusSq - KUtil::Square(vD0);
            }
        }

        // vD1 is max.
        else if (vD1 >= vD0 && vD1 >= vD2)
        {
            if (vD0 >= vD2) // Edges are 0 and 1.
            {
                // Edges are 0 and 1.
                edge1 = d0;
                edge2 = d1;
                t1 = glm::dot(edge1, edge2);
                if (vD0 >= t1 * vD1) outInfo.collisionType = COLLISION_VERTEX0;
            }
            else // Edges are 1 and 2.
            {
                edge1 = d1;
                edge2 = d2;
                t1 = glm::dot(edge1, edge2);
                if (vD2 >= t1 * vD1) outInfo.collisionType = COLLISION_VERTEX1;
            }
            if (outInfo.collisionType == COLLISION_UNDEF)
            {
                //if (vD1 > vN) return false;
                outInfo.collisionType = COLLISION_EDGE1;
                dist = radiusSq - KUtil::Square(vD1);
            }
        }

        // vD2 is max.
        else
        {
            if (vD1 >= vD0) // Edges are 1 and 2.
            {
                edge1 = d1;
                edge2 = d2;
                t1 = glm::dot(edge1, edge2);
                if (vD1 >= t1 * vD2) outInfo.collisionType = COLLISION_VERTEX1;
            }
            else // Edges are 2 and 0.
            {
                edge1 = d2;
                edge2 = d0;
                t1 = glm::dot(edge1, edge2);
                if (vD0 >= t1 * vD2) outInfo.collisionType = COLLISION_VERTEX2;
            }
            if (outInfo.collisionType == COLLISION_UNDEF)
            {
                //if (vD2 > vN) return false;
                outInfo.collisionType = COLLISION_EDGE2;
                dist = radiusSq - KUtil::Square(vD2);
            }
        }

        // Do vertex.
        if (outInfo.collisionType == COLLISION_VERTEX0 || outInfo.collisionType == COLLISION_VERTEX1 || outInfo.collisionType == COLLISION_VERTEX2)
        {
            float c0, c1;
            if (outInfo.collisionType == COLLISION_VERTEX0)
            {
                c0 = (((t1 * vD1) - vD0)) / (KUtil::Square(t1) - 1.0);
                c1 = vD1 - (c0 * t1);
            }
            else if (outInfo.collisionType == COLLISION_VERTEX1)
            {
                c0 = (((t1 * vD2) - vD1)) / (KUtil::Square(t1) - 1.0);
                c1 = vD2 - (c0 * t1);
            }
            else
            {
                c0 = (((t1 * vD0) - vD2)) / (KUtil::Square(t1) - 1.0);
                c1 = vD0 - (c0 * t1);
            }
            glm::vec3 tmp = c0 * edge1 + c1 * edge2;
            float distSq = glm::dot(tmp, tmp);
            //if (distSq > KUtil::Square(vN) || distSq >= radiusSq) return false;
            dist = glm::sqrt(radiusSq - distSq) - vN;
        }

        // Do edge.
        else
        {
            dist = glm::sqrt(dist) - vN;
        }

    }
    else
    {

        // Face test was used, p = r - v * n.
        dist = radius - vN;
        outInfo.collisionType = COLLISION_FACE;

    }
    float maxDist = triangleThickness * scale;
    if (dist < 0.0f || dist > maxDist) return false;
    outInfo.distance = dist;
    return true;

}

bool KModel::CalcPenetrationPoint(KModelTriangle& tri, const glm::vec3& pos, float thickness, KModelPenetrationInfo& outInfo)
{
    ZoneScopedN("KModel::CalcPenetrationPoint");

    glm::vec3 p = IndexToPoint(tri.originPointIndex);
    glm::vec3 v = pos - p;

    // Try desting distance with each edge.
    glm::vec3 d0 = IndexToVector(tri.direction0VectorIndex);
    float d0Dot = glm::dot(v, d0);
    if (d0Dot < 0.0f) return false;

    glm::vec3 d1 = IndexToVector(tri.direction1VectorIndex);
    float d1Dot = glm::dot(v, d1);
    if (d1Dot < 0.0f) return false;

    glm::vec3 d2 = IndexToVector(tri.direction2VectorIndex);
    float d2Dot = glm::dot(v, d2) - tri.length;
    if (d2Dot < 0.0f) return false;

    // Normal test.
    glm::vec3 n = IndexToVector(tri.normalVectorIndex);
    float dist = -glm::dot(n, v);
    float maxDist = thickness * triangleThickness;
    if (dist < 0.0f || dist > maxDist) return false;

    // Return distance info.
    outInfo.collisionType = COLLISION_FACE;
    outInfo.distance = dist;
    return true;

}

bool KModel::CheckSphere(const glm::vec3& pos, float radius, float scale, unsigned int maxResults, std::vector<KModelCheckResult>& results)
{
    ZoneScopedN("KModel::CheckSphere");

    glm::vec3 newPos = pos - translation;
    // TODO!!!
}

bool KModel::CheckPoint(const glm::vec3& pos, float thickness, unsigned int maxResults, std::vector<KModelCheckResult>& results)
{
    ZoneScopedN("KModel::CheckPoint");

    glm::vec3 newPos = pos - translation;
    KModelPenetrationInfo info;
    std::vector<std::vector<unsigned int>*> tris;
    octree.GetTriangles(newPos, thickness, tris);
    for (auto triSet : tris)
    for (auto triInd : *triSet)
    {
        if (CalcPenetrationPoint(triangles[triInd], newPos, thickness, info))
        {
            results.push_back(KModelCheckResult(triangles[triInd], info));
            if (results.size() >= maxResults) return true;
        }
    }
    return results.size() > 0;
}