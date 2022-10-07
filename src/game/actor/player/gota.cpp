#include "gota.h"

#include <tracy/Tracy.hpp>

std::unique_ptr<GActorBase> GActorPlayerGota::Spawn(const GSpawnInfo& info)
{
    ZoneScopedN("GActorPlayerGota::Spawn");

    if (!GActor::VerifyArgs(info, 1, 0, 0)) return nullptr;
    return std::make_unique<GActorPlayerGota>(info.scenario, info.actorID, info.vecs[0]);
}

GActorPlayerGota::GActorPlayerGota(GScenario& scenario, std::string actorID, glm::vec3 pos) : GActorPlayer(scenario, actorID, pos)
{
    ZoneScopedN("GActorPlayer::GActorPlayer");
}