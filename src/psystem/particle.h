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
    unsigned int lifetime;
    unsigned int age;
    float scale;
    float scaleMult;
    float angle;
    float angleSpeed;
    glm::vec3 color;
    unsigned int altLenInv;
    unsigned int lifetimeInv;
    unsigned int alpha;
    unsigned int alphaMult;
    std::shared_ptr<PTexture> sprite;

    // Initialize a particle.
    void Init(PSystem* system, int index, int total);

    // Update a particle.
    void Update(PSystem* system, std::stack<PParticle*>& freeParticles);

};