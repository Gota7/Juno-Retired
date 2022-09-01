#pragma once

#include <string>

// Forward declare.
struct GScenario;

// Base structure for actors.
struct GActorBase
{
    GScenario& scenario;
    std::string actorID;

    // Construct the base actor.
    GActorBase(GScenario& scenario, std::string actorID) : scenario(scenario), actorID(actorID) {}

    // Load resources. True if success.
    virtual bool LoadResources() { return true; }

    // Cleanup resources.
    virtual void CleanupResources() {}

    // Render the actor base. True if success.
    virtual bool Render() { return true; }

    // Update the base logic. True if success.
    virtual bool Update() { return true; }

    // Kill the actor.
    virtual void Kill() {}

    // Destructor.
    ~GActorBase() { CleanupResources(); }

};