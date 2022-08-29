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

// Penetration type.
enum KPenetrationType
{
    PENETRATION_FLOOR,
    PENETRATION_WALL,
    PENETRATION_CEILING
};

// Collider information.
struct KModelPenetrationInfo
{
    glm::vec3 penetration;
    KPenetrationType type;
    bool faceCollision;
};

// JKCL model file format.
struct KModel : KMesh
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
    glm::mat4 matrix; // General matrix.
    glm::mat4 invMatrix; // Inverse matrix.

    // Create a new collision model.
    KModel(std::string path, glm::mat4 matrix = glm::mat4(1.0f));

    // Get or add a point.
    unsigned int GetOrAddPoint(glm::vec3 pt);

    // Get or add a vector.
    unsigned int GetOrAddVec(glm::vec3 vec);

    // Import a node.
    void ImportNode(const aiScene* scene, aiNode* node);

    // Import a mesh.
    void ImportMesh(const aiScene* scene, aiMesh* mesh);

    // Convert to JModel for convenience.
    std::unique_ptr<JModel> ToJModel(JShader& shader);

    // Calculate penetration of a sphere relative to collision coordinates. Returns if it succeeds.
    bool CalcPenetration(KModelTriangle& tri, const glm::vec3& pos, float radius, const glm::vec3& gravDir, KModelPenetrationInfo* outInfo);

    // Make a sphere stop colliding with the mesh. Make sure to only pass one floor collision at a time.
    static void Unpenetrate(glm::vec3& pos, std::vector<KModelPenetrationInfo>& penetrations);

    // V-functions.
    virtual glm::vec3 Position();
    virtual glm::vec3 Range();
    virtual void Uncollide(glm::vec3& pos, float radius, const glm::vec3& gravDir);

};