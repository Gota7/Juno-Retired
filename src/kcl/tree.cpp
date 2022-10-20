#include "tree.h"
#include "util.h"

#include <tracy/Tracy.hpp>

KTreeNodeParent::KTreeNodeParent(std::unique_ptr<KTreeNode> left, std::unique_ptr<KTreeNode> right)
{
    ZoneScopedN("KTreeNodeParent::KTreeNodeParent");

    KUtil::CombinedBoxRange(left->boxPos, left->boxSize, right->boxPos, right->boxSize, &boxPos, &boxSize);
    this->left = std::move(left);
    this->right = std::move(right);
}

std::unique_ptr<KMesh> KTreeNodeParent::AddMesh(std::unique_ptr<KMesh> mesh)
{
    ZoneScopedN("KTreeNodeParent::AddMesh");

    // Add mesh to box with less combined surface area.
    glm::vec3 pos, size;
    float saLeft, saRight;
    std::unique_ptr<KMesh> recall;
    pos = mesh->Position();
    glm::vec3 range = mesh->Range();
    KUtil::CombinedBoxRange(pos - range, range * 2.0f, left->boxPos, left->boxSize, &pos, &size);
    saLeft = KUtil::SurfaceArea(size);
    KUtil::CombinedBoxRange(pos - range, range * 2.0f, right->boxPos, right->boxSize, &pos, &size);
    saRight = KUtil::SurfaceArea(size);
    if (saLeft < saRight)
    {
        if (recall = left->AddMesh(std::move(mesh)), recall)
        {
            left = std::make_unique<KTreeNodeParent>(std::move(left), std::make_unique<KTreeNodeLeaf>(std::move(recall)));
        }
    }
    else
    {
        if (recall = right->AddMesh(std::move(mesh)), recall)
        {
            right = std::make_unique<KTreeNodeParent>(std::move(right), std::make_unique<KTreeNodeLeaf>(std::move(recall)));
        }
    }
    return nullptr; // Nothing to recall.

}

void KTreeNodeParent::FindIntersections(const glm::vec3& pos, std::vector<KMesh*>& out)
{
    ZoneScopedN("KTreeNodeParent::FindIntersections");

    if (KUtil::PointInBox(pos, left->boxPos, left->boxSize)) left->FindIntersections(pos, out);
    if (KUtil::PointInBox(pos, right->boxPos, right->boxSize)) right->FindIntersections(pos, out);
}

KTreeNodeLeaf::KTreeNodeLeaf(std::unique_ptr<KMesh> mesh)
{
    ZoneScopedN("KTreeNodeLeaf::KTreeNodeLeaf");

    glm::vec3 pos = mesh->Position();
    glm::vec3 range = mesh->Range();
    boxPos = pos - range;
    boxSize = range * 2.0f;
    this->mesh = std::move(mesh);
}

void KTreeNodeLeaf::FindIntersections(const glm::vec3& pos, std::vector<KMesh*>& out)
{
    ZoneScopedN("KTreeNodeLeaf::KTreeNodeParent");

    out.push_back(mesh.get()); // Guaranteed to be intersecting since checks done in advance.
}

KTree::KTree(std::vector<std::unique_ptr<KMesh>> meshes)
{
    ZoneScopedN("KTree::KTree");

    for (auto& mesh : meshes)
    {
        AddMesh(std::move(mesh));
    }
}

void KTree::AddMesh(std::unique_ptr<KMesh> mesh)
{
    ZoneScopedN("KTree::AddMesh");

    if (!root)
    {
        root = std::make_unique<KTreeNodeLeaf>(std::move(mesh));
        return;
    }
    std::unique_ptr<KMesh> recall;
    if (recall = root->AddMesh(std::move(mesh)), recall)
    {
        root = std::make_unique<KTreeNodeParent>(std::move(root), std::make_unique<KTreeNodeLeaf>(std::move(recall)));
    }
}

std::vector<KMesh*> KTree::FindIntersections(const glm::vec3& pos)
{
    ZoneScopedN("KTree::FindIntersections");

    std::vector<KMesh*> ret;
    if (root && KUtil::PointInBox(pos, root->boxPos, root->boxSize)) root->FindIntersections(pos, ret);
    return ret;
}