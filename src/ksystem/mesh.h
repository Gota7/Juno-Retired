#pragma once

#include "../jsystem.h"

// General mesh collider. Assumed to be stationary.
struct KMesh
{
    glm::vec3 pos; // Position of the mesh collider.
    glm::vec3 range; // Bounding box surrounding position.
};