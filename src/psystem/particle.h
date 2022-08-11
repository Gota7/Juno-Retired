#pragma once

#include "../jsystem.h"

// Standard particle.
struct PParticle
{
    glm::vec3 pos;
    glm::vec3 offset;
    glm::vec3 vel;
    int lifetime;
    int age;
    float scale;
    float scaleMult;
    float angle;
    float angleSpeed;
    glm::vec4 color;
    int altLenInv;
    int lifetimeInv;
    int alpha;
    int alphaMult;
    // TODO: PARTICLE TEXTURE!!!
};