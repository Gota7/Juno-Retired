#include "spawnTable.h"

#include "obj/lameSphere.h"

std::map<std::string, SpawnFunc> GSpawnTable::ACTOR_SPAWN_TABLE =
{
    { "LameSphere", GActorLameSphere::Spawn }
};

std::unique_ptr<GActorBase> GSpawnTable::Spawn(const GSpawnInfo& spawnInfo)
{
    if (ACTOR_SPAWN_TABLE.find(spawnInfo.actorID) == ACTOR_SPAWN_TABLE.end())
    {
        std::cout << "ERROR: Actor " + spawnInfo.actorID + " does not exist." << std::endl;
        return nullptr;
    }
    return ACTOR_SPAWN_TABLE[spawnInfo.actorID](spawnInfo);
}