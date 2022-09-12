#pragma once

#include "tree.h"
#include "../jsystem.h"

// Collision body.
struct KBody
{
    glm::vec3 pos = glm::vec3(0.0f);
    glm::vec3 vel = glm::vec3(0.0f);
    glm::vec3 acc = glm::vec3(0.0f);
    glm::vec3 velMax = glm::vec3(10.0f, 10.0f, 10.0f);
    float radius;
    bool colliderEnabled = true;
    bool hitLastFrame = false;

    // Create a new body.
    KBody(float radius) : radius(radius) {}

    // Update the body position.
    void UpdatePhysics(float dt);

    // Respond to collision.
    void RespondToCollision(KTree& tree, const glm::vec3& gravDir);

    // Update the body. Call this one only.
    void Update(KTree& tree, const glm::vec3& gravDir, float dt);

};