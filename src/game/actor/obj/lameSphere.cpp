#include "lameSphere.h"

GActorLameSphere::GActorLameSphere(const GSpawnInfo& info) : GActor(info.scenario, info.actorID, info.floats[0])
{
    body.pos = info.vecs[0];
}

bool GActorLameSphere::BeforeUpdate()
{
    body.vel = grav * 0.05f; // Set constant speed to gravity.
    return true;
}