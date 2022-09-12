#pragma once

#include <string>
#include <iostream>

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

    // Render the actor base. True if success.
    virtual bool Render() { return true; }

    // Update the base logic. True if success.
    virtual bool Update() { return true; }

    // Virtual destructor.
    virtual ~GActorBase() {}

};