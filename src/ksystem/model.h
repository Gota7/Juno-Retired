#pragma once

#include "../jsystem.h"

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
struct KModel
{
    std::vector<glm::vec3> points; // Collection of origin points.
    std::vector<glm::vec3> vectors; // Direction vectors. ALL DIRECTIONS SHOULD BE NORMALIZED!
    std::vector<KModelTriangle> triangles; // Actual triangles that make up the model.
    // TODO: OCTREE!
    glm::mat4 matrix;
    glm::mat4 invMatrix;

    // Create a new collision model.
    KModel(std::string path, glm::mat4 matrix = glm::mat4(1.0f));

    // Import a node.
    void ImportNode(const aiScene* scene, aiNode* node);

    // Import a mesh.
    void ImportMesh(const aiScene* scene, aiMesh* mesh);

    // Calculate penetration of a sphere relative to collision coordinates. Returns if it succeeds.
    bool CalcPenetration(KModelTriangle& tri, const glm::vec3& pos, float radius, const glm::vec3& gravDir, KModelPenetrationInfo* outInfo);

    // Make a sphere stop colliding with the mesh. Make sure to only pass one floor collision at a time.
    static void Unpenetrate(glm::vec3& pos, std::vector<KModelPenetrationInfo> penetrations);

};