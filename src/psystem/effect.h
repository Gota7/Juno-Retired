#pragma once

#include "particle.h"

// Particle effect.
struct PEffect
{

    // Process a particle.
    virtual void Process(PParticle& particle, glm::vec3& vel) {}

};