#pragma once

#include "../../../particle.h"
#include "../actor.h"

// Player base.
struct GActorPlayer : GActor
{
    PSystem* system; // Particle system.
    glm::vec3 playerXAxis; // X axis for player.
    glm::vec3 playerZAxis; // Z axis for player.

    // Make a player.
    GActorPlayer(GScenario& scenario, std::string actorID, glm::vec3 pos);

    // Update player movement axes.
    void UpdatePlayerMovementAxes();

    // Update the camera. Returns true if there is a change.
    bool UpdateCamera();

    // Vfunctions.
    virtual bool LoadResources() override;
    virtual bool BeforeUpdate() override;
    virtual bool AfterUpdate() override;
    virtual ~GActorPlayer() override;

};