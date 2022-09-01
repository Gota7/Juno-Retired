#pragma once

#include "../../kcl.h"
#include "../../jsystem.h"
#include "base.h"

// An actual actor.
struct GActor : GActorBase
{
    KBody body; // Collider body.
    glm::vec3 grav = glm::vec3(0.0f, -1.0f, 0.0f); // Gravity.

    // Create a new actor.
    GActor(GScenario& scenario, std::string actorID, float bodyRadius) : GActorBase(scenario, actorID), body(bodyRadius) {}

    // Vfunctions.
    virtual bool BeforeUpdate() { return true; }
    virtual bool Update();
    virtual bool AfterUpdate() { return true; }

};