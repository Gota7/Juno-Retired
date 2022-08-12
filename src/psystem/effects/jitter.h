#pragma once

#include "../effect.h"

// Jitter effect.
struct PEffectJitter
{
    glm::vec3 mag;
    float period;
    float lastPeriod;

    PEffectJitter(glm::vec3 mag, float period = 0) : mag(mag), period(period), lastPeriod(0) {}
    virtual void Process(PParticle& particle, glm::vec3& vel);
};