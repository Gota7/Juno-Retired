#pragma once

#include "player.h"

// Gota character.
struct GActorPlayerGota : GActorPlayer
{

    // Spawn Gota.
    static std::unique_ptr<GActorBase> Spawn(const GSpawnInfo& info);

    // Make Gota.
    GActorPlayerGota(GScenario& scenario, std::string actorID, glm::vec3 pos);

};