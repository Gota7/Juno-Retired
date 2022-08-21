#pragma once

#include "../effect.h"

// Turn axis.
enum PEffectTurnAxis
{
    PEFFECT_TURN_AXIS_X,
    PEFFECT_TURN_AXIS_Y,
    PEFFECT_TURN_AXIS_Z
};

// Turn effect.
struct PEffectTurn
{
    float angleSpeed;
    PEffectTurnAxis axis;

    PEffectTurn(float angleSpeed, PEffectTurnAxis axis) : angleSpeed(angleSpeed), axis(axis) {}
    virtual void Process(PParticle& particle, glm::vec3& vel);
};