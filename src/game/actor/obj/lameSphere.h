#pragma once

#include "../actor.h"
#include "../spawnInfo.h"

// A lame sphere. It doesn't do anything except fall towards gravity and collides with meshes.
struct GActorLameSphere : GActor
{

    // Spawn the sphere.
    GActorLameSphere(const GSpawnInfo& info);

    // Vfunctions.
    virtual bool BeforeUpdate();

};