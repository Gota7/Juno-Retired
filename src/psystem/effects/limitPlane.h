#pragma once

#include "../effect.h"

// Nehaviour.
enum PEffectLimitPlaneBehaviour
{
    PEFFECT_LIMITPLANE_KILL,
    PEFFECT_LIMITPLANE_CHANGE_SPEED
};

// Limit plane effect.
struct PEffectLimitPlane
{
    PEffectLimitPlaneBehaviour behaviour;
    float posY;
    float reverseSpeedMult;

    PEffectLimitPlane(PEffectLimitPlaneBehaviour behaviour, float posY, float reverseSpeedMult) : behaviour(behaviour), posY(posY), reverseSpeedMult(reverseSpeedMult) {}
    virtual void Process(PParticle& particle, glm::vec3& vel);
};