#pragma once

#include "../jsystem.h"

// General mesh collider. Assumed to be stationary.
struct KMesh
{

    // Get the position.
    virtual glm::vec3 Position() { return glm::vec3(0.0f); }

    // Get the collider range.
    virtual glm::vec3 Range() { return glm::vec3(1.0f, 1.0f, 1.0f); }

    // Uncollide from a mesh.
    virtual void Uncollide(glm::vec3& pos, float radius, const glm::vec3& gravDir) {}

};