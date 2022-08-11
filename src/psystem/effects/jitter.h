#pragma once

#include "../effect.h"

// Jitter effect.
struct PEffectJitter
{
    glm::vec3 mag;
    unsigned int period;

    PEffectJitter(glm::vec3 mag, unsigned int period = 1) : mag(mag), period(period) {}
    virtual void Process(PParticle& particle, glm::vec3& vel);
};