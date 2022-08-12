#pragma once

#include "../jsystem.h"
#include "texture.h"
#include <stack>

struct PSystem;

// Standard particle.
struct PParticle
{
    glm::vec3 pos;
    glm::vec3 offset;
    glm::vec3 vel;
    float lifetime;
    float age;
    float scale;
    float scaleMult;
    float angle;
    float angleSpeed;
    glm::vec3 color;
    unsigned int altLenInv;
    float lifetimeInv;
    unsigned int alpha;
    unsigned int alphaMult;
    PTexture sprite;

    // Initialize a particle.
    PParticle(PSystem* system, int index, int total);

    // Update a particle.
    void Update(PSystem* system);

};