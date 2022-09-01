#pragma once

#include "base.h"
#include "spawnInfo.h"

// Spawn table for actors.
struct GSpawnTable
{

    // Spawn an actor.
    static std::unique_ptr<GActorBase> Spawn(const GSpawnInfo& spawnInfo);

};