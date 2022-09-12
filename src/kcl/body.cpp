#include "body.h"

void KBody::UpdatePhysics(float dt)
{
    if (glm::abs(vel.x) > velMax.x) vel.x = glm::sign(vel.x) * velMax.x;
    if (glm::abs(vel.y) > velMax.y) vel.y = glm::sign(vel.y) * velMax.y;
    if (glm::abs(vel.z) > velMax.z) vel.z = glm::sign(vel.z) * velMax.z;
    vel += acc * dt;
    pos += vel * dt;
}

void KBody::RespondToCollision(KTree& tree, const glm::vec3& gravDir)
{
    hitLastFrame = false;
    std::vector<KMesh*> meshes = tree.FindIntersections(pos);
    for (auto mesh : meshes)
    {
        if (mesh->Uncollide(pos, radius, gravDir)) hitLastFrame = true;
    }
}

void KBody::Update(KTree& tree, const glm::vec3& gravDir, float dt)
{
    UpdatePhysics(dt);
    if (colliderEnabled) RespondToCollision(tree, gravDir);
}