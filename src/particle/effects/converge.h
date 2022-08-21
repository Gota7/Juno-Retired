#pragma once

#include "../effect.h"

// Converge effect.
struct PEffectConverge
{
    glm::vec3 offset;
    float mag;

    PEffectConverge(glm::vec3 offset, float mag) : offset(offset), mag(mag) {}
    virtual void Process(PParticle& particle, glm::vec3& vel);
};