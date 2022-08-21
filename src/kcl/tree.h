#pragma once

#include "mesh.h"

// TODO: TREE REBALANCING!!! SEE THIS: https://appliedgo.net/balancedtree/

// Node of a tree.
struct KTreeNode
{
    glm::vec3 boxPos; // Base position of box in 3d space.
    glm::vec3 boxSize; // Size of box in 3d space.

    // Add a mesh to the node. Returns the mesh back if it isn't successful.
    virtual std::unique_ptr<KMesh> AddMesh(std::unique_ptr<KMesh> mesh) { return std::move(mesh); }

    // Search for intersecting meshes.
    virtual void FindIntersections(const glm::vec3& pos, std::vector<KMesh*>& out) {}

};

// Parent node of a tree.
struct KTreeNodeParent : KTreeNode
{
    std::unique_ptr<KTreeNode> left; // Left node.
    std::unique_ptr<KTreeNode> right; // Right node.

    // Create a parent node from two more nodes.
    KTreeNodeParent(std::unique_ptr<KTreeNode> left, std::unique_ptr<KTreeNode> right);

    // Virtual functions.
    virtual std::unique_ptr<KMesh> AddMesh(std::unique_ptr<KMesh> mesh);
    virtual void FindIntersections(const glm::vec3& pos, std::vector<KMesh*>& out);

};

// Leaf node of a tree.
struct KTreeNodeLeaf : KTreeNode
{
    std::unique_ptr<KMesh> mesh; // Child mesh.

    // Create a leaf node from a child mesh.
    KTreeNodeLeaf(std::unique_ptr<KMesh> mesh);

    // Virtual functions.
    virtual void FindIntersections(const glm::vec3& pos, std::vector<KMesh*>& out);

};

// KCL tree to partition 3d space of collision meshes. This allows us to efficiently find intersections.
struct KTree
{
    std::unique_ptr<KTreeNode> root = nullptr; // Root of the tree.

    // Construct a tree.
    KTree(std::vector<std::unique_ptr<KMesh>> meshes);

    // Add a mesh to the tree.
    void AddMesh(std::unique_ptr<KMesh> mesh);

    // Search for intersecting meshes.
    std::vector<KMesh*> FindIntersections(const glm::vec3& pos);

};