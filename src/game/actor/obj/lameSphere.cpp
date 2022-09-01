#include "lameSphere.h"

GActorLameSphere::GActorLameSphere(const GSpawnInfo& info) : GActor(info.scenario, info.actorID, info.floats[0])
{
    body.pos = info.vecs[0];
}