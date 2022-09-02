#pragma once

#include "../../../particle.h"
#include "../actor.h"
#include "../spawnInfo.h"

// A lame sphere. It doesn't do anything except fall towards gravity and collides with meshes.
struct GActorLameSphere : GActor
{
    PSystem* system;

    // Spawn the sphere.
    static std::unique_ptr<GActorBase> Spawn(const GSpawnInfo& info);

    // Make a sphere.
    GActorLameSphere(GScenario& scenario, std::string actorID, glm::vec3 pos);
    ~GActorLameSphere();

    // Vfunctions.
    virtual bool LoadResources();
    virtual bool BeforeUpdate();

};