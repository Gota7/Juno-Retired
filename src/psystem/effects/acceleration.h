#pragma once

#include "../effect.h"

// Acceleration effect.
struct PEffectAcceleration
{
    glm::vec3 accel;

    PEffectAcceleration(glm::vec3 accel) : accel(accel) {}
    virtual void Process(PParticle& particle, glm::vec3& vel);
};