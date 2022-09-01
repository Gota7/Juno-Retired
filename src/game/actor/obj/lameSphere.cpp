#include "lameSphere.h"

std::unique_ptr<GActorBase> GActorLameSphere::Spawn(const GSpawnInfo& info)
{
    if (!GActor::VerifyArgs(info, 1, 1, 0)) return nullptr;
    return std::make_unique<GActorLameSphere>(info.scenario, info.actorID, info.vecs[0], info.floats[0]);
}

GActorLameSphere::GActorLameSphere(GScenario& scenario, std::string actorID, glm::vec3 pos, float radius) : GActor(scenario, actorID, radius)
{
    body.pos = pos;
}

bool GActorLameSphere::BeforeUpdate()
{
    body.vel = grav * 0.05f; // Set constant speed to gravity.
    return true;
}