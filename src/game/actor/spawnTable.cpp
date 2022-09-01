#include "spawnTable.h"

#include "obj/lameSphere.h"

#include <functional>

#define MK_SPAWN(name) [](const GSpawnInfo& info) -> std::unique_ptr<GActorBase> { return std::make_unique<name>(info); }

std::map<std::string, std::function<std::unique_ptr<GActorBase>(const GSpawnInfo&)>> ACTOR_SPAWN_TABLE =
{
    { "LameSphere", MK_SPAWN(GActorLameSphere) }
};

std::unique_ptr<GActorBase> GSpawnTable::Spawn(const GSpawnInfo& spawnInfo)
{
    return ACTOR_SPAWN_TABLE[spawnInfo.actorID](spawnInfo);
}