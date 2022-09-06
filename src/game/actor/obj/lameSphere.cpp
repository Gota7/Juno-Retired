#include "lameSphere.h"
#include "../../game.h"
#include "../../scenario.h"

std::unique_ptr<GActorBase> GActorLameSphere::Spawn(const GSpawnInfo& info)
{
    if (!GActor::VerifyArgs(info, 1, 0, 0)) return nullptr;
    return std::make_unique<GActorLameSphere>(info.scenario, info.actorID, info.vecs[0]);
}

GActorLameSphere::GActorLameSphere(GScenario& scenario, std::string actorID, glm::vec3 pos) : GActor(scenario, actorID, 1.0f)
{
    body.pos = pos;
}

GActorLameSphere::~GActorLameSphere()
{
    system->stopped = true;
}

bool GActorLameSphere::LoadResources()
{
    system = &scenario.game.particleMgr->AddSystem("LameSphere", body.pos, nullptr);
    return true;
}

bool GActorLameSphere::BeforeUpdate()
{
    body.vel = grav * 0.5f + glm::vec3(-0.08f, 0.0f, 0.08f); // Set constant speed to gravity and more.
    system->pos = body.pos;
    return true;
}