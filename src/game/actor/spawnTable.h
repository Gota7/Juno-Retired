#pragma once

#include "base.h"
#include "spawnInfo.h"

using SpawnFunc = std::unique_ptr<GActorBase>(*)(const GSpawnInfo&);

// Spawn table for actors.
struct GSpawnTable
{
    static std::map<std::string, SpawnFunc> ACTOR_SPAWN_TABLE; // Universal spawn table.

    // Spawn an actor.
    static std::unique_ptr<GActorBase> Spawn(const GSpawnInfo& spawnInfo);

};