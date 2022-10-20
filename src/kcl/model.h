#pragma once

#include "../jsystem.h"
#include "mesh.h"
#include "octree.h"

// Model triangles.
// See https://www.youtube.com/watch?v=6hUK1Wbajt4&list=PL0TeYaSr_hNedZtktHufaFNO1usnQOuom&index=5
struct KModelTriangle
{
    float length;
    unsigned int originPointIndex; // Origin point.
    unsigned int normalVectorIndex; // Normal/up direction.
    unsigned int direction0VectorIndex; // Direction left of point.
    unsigned int direction1VectorIndex; // Direction right of point.
    unsigned int direction2VectorIndex; // Outward/forward direction.
    unsigned int terrainType; // Index of material in linked CLPS.
};

// Collision type.
enum KModelTriangleCollisionType
{
    COLLISION_UNDEF,
    COLLISION_FACE,
    COLLISION_EDGE0,
    COLLISION_EDGE1,
    COLLISION_EDGE2,
    COLLISION_VERTEX0,
    COLLISION_VERTEX1,
    COLLISION_VERTEX2
};

// Collider information.
struct KModelPenetrationInfo
{
    float distance;
    KModelTriangleCollisionType collisionType;
};

// Check result.
struct KModelCheckResult
{
    KModelTriangle& tri;
    KModelPenetrationInfo info;

    KModelCheckResult(KModelTriangle& tri, KModelPenetrationInfo info) : tri(tri), info(info) {}
};

// JKCL model file format.
struct KModel
{
    std::vector<glm::vec3> points; // Collection of origin points.
    std::vector<glm::vec3> vectors; // Direction vectors. ALL DIRECTIONS SHOULD BE NORMALIZED!
    std::vector<KModelTriangle> triangles; // Actual triangles that make up the model.
    KOctree octree; // For quick collision lookup.
    std::map<size_t, unsigned int> pointIndices; // Used during construction to optimize points.
    std::map<size_t, unsigned int> vectorIndices; // Used during construction to optimize vectors.
    std::vector<std::tuple<glm::vec3, glm::vec3, glm::vec3, glm::vec3>> triangleData; // Used during construction for octree generation.
    std::map<unsigned int, std::vector<unsigned int>> matToTriangleMeshes; // Convert a material index to a collection of triangles.
    unsigned int numMaterials; // Number of materials in the model. Used to convert to JModel.
    glm::vec3 translation; // Translation of the collision in world space.
    glm::mat4 matrix; // General matrix.
    glm::mat4 invMatrix; // Inverse matrix.
    float triangleThickness; // Thickness to round each triangle.

    // Create a new collision model.
    KModel(std::string path, glm::mat4 matrix = glm::mat4(1.0f), float triangleThickness = 0.005f);

    // Get or add a point.
    unsigned int GetOrAddPoint(glm::vec3 pt);

    // Get or add a vector.
    unsigned int GetOrAddVec(glm::vec3 vec);

    // Convert an index to a point.
    inline glm::vec3 IndexToPoint(unsigned int pointIndex) { return points[pointIndex]; }

    // Convert an index to a vector.
    inline glm::vec3 IndexToVector(unsigned int vectorIndex) { return vectors[vectorIndex]; }

    // Import a node.
    void ImportNode(const aiScene* scene, aiNode* node);

    // Import a mesh.
    void ImportMesh(const aiScene* scene, aiMesh* mesh);

    // Get a vertex position.
    glm::vec3 GetVertex(KModelTriangle& tri, int vertexNum);

    // Convert to JModel for convenience.
    std::unique_ptr<JModel> ToJModel(JShader& shader);

    // Calculate penetration of an arrow. Returns if it succeeds.
    bool CalcPenetrationArrow(KModelTriangle& tri, const glm::vec3& pos, const glm::vec3& dir, KModelPenetrationInfo& outInfo);

    // Calculate penetration of a sphere. Returns if it succeeds.
    bool CalcPenetrationSphere(KModelTriangle& tri, const glm::vec3& pos, float radius, float scale, KModelPenetrationInfo& outInfo);

    // Calculate penetration of a point. Returns if it succeeds.
    bool CalcPenetrationPoint(KModelTriangle& tri, const glm::vec3& pos, float thickness, KModelPenetrationInfo& outInfo);

    // TODO: ARROW CHECKING!!!

    // Check collision with a sphere. Returns if it succeeds.
    bool CheckSphere(const glm::vec3& pos, float radius, float scale, unsigned int maxResults, std::vector<KModelCheckResult>& results);

    // Check collision with a point. Returns if it succeeds.
    bool CheckPoint(const glm::vec3& pos, float thickness, unsigned int maxResults, std::vector<KModelCheckResult>& results);

};