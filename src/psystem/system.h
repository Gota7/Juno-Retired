#pragma once

#include "effect.h"
#include "glitter.h"
#include "spawnInfo.h"
#include "transition.h"

// Particle system definition.
struct PSystemDefinition
{
    PSpawnInfo spawnInfo;
    std::vector<std::unique_ptr<PTransition>> transitions;
    std::unique_ptr<PGlitter> glitter;
    std::vector<std::unique_ptr<PEffect>> effects;
};

// System. TODO!!!
struct PSystem
{
    std::unique_ptr<PSystemDefinition> definition;
};