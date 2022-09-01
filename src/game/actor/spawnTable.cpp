#include "spawnTable.h"

#include "obj/lameSphere.h"

#include <functional>

#define MK_SPAWN(name) [](const GSpawnInfo& info) -> std::unique_ptr<GActorBase> { return std::make_unique<name>(info); }
#define MK_PARAMS(minVecs, minFloats, minStrings) [](const GSpawnInfo& info) -> bool { \
    if (info.vecs.size() < minVecs) { std::cout << "ERROR: Initializing " + info.actorID + " failed. Expected at least " + std::to_string(minVecs) + " vector args but got " + std::to_string(info.vecs.size()) + "." << std::endl; return false; } \
    if (info.floats.size() < minFloats) { std::cout << "ERROR: Initializing " + info.actorID + " failed. Expected at least " + std::to_string(minFloats) + " float args but got " + std::to_string(info.floats.size()) + "." << std::endl; return false; } \
    if (info.strings.size() < minStrings) { std::cout << "ERROR: Initializing " + info.actorID + " failed. Expected at least " + std::to_string(minStrings) + " string args but got " + std::to_string(info.strings.size()) + "." << std::endl; return false; } \
    return true; \
}

std::map<std::string, std::function<std::unique_ptr<GActorBase>(const GSpawnInfo&)>> ACTOR_SPAWN_TABLE =
{
    { "LameSphere", MK_SPAWN(GActorLameSphere) }
};

std::map<std::string, std::function<bool(const GSpawnInfo&)>> ACTOR_PARAMS_SATISFIED =
{
    { "LameSphere", MK_PARAMS(1, 1, 0) }
};

std::unique_ptr<GActorBase> GSpawnTable::Spawn(const GSpawnInfo& spawnInfo)
{
    if (ACTOR_PARAMS_SATISFIED.find(spawnInfo.actorID) == ACTOR_PARAMS_SATISFIED.end() || ACTOR_SPAWN_TABLE.find(spawnInfo.actorID) == ACTOR_SPAWN_TABLE.end())
    {
        std::cout << "ERROR: Actor " + spawnInfo.actorID + " does not exist." << std::endl;
        return nullptr;
    }
    if (!ACTOR_PARAMS_SATISFIED[spawnInfo.actorID](spawnInfo)) return nullptr;
    return ACTOR_SPAWN_TABLE[spawnInfo.actorID](spawnInfo);
}