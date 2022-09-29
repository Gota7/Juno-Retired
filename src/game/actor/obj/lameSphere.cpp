#include "lameSphere.h"
#include "../../game.h"
#include "../../scenario.h"

#include <tracy/Tracy.hpp>

std::unique_ptr<GActorBase> GActorLameSphere::Spawn(const GSpawnInfo& info)
{
    ZoneScopedN("GActorLameSphere::Spawn");

    if (!GActor::VerifyArgs(info, 1, 0, 0)) return nullptr;
    return std::make_unique<GActorLameSphere>(info.scenario, info.actorID, info.vecs[0]);
}

GActorLameSphere::GActorLameSphere(GScenario& scenario, std::string actorID, glm::vec3 pos) : GActor(scenario, actorID, 1.0f)
{
    ZoneScopedN("GActorLameSphere::GActorLameSphere");

    body.pos = pos;
}

bool GActorLameSphere::LoadResources()
{
    ZoneScopedN("GActorLameSphere::LoadResources");

    system = &scenario.game.particleMgr->AddSystem("LameSphere", body.pos, nullptr);
    return true;
}

bool GActorLameSphere::BeforeUpdate()
{
    ZoneScopedN("GActorLameSphere::BeforeUpdate");

    body.vel = grav * 0.5f + glm::vec3(0.18f, 0.0f, -0.18f); // Set constant speed to gravity and more.
    system->pos = body.pos;
    if (body.hitLastFrame) return false;
    return true;
}

GActorLameSphere::~GActorLameSphere()
{
    ZoneScopedN("GActorLameSphere::~GActorLameSphere");

    system->stopped = true;
}