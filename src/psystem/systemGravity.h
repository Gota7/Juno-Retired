#pragma once

#include "../rsystem.h"
#include "system.h"

// Special system type for gravity tests.
struct PSystemGravity : PSystem
{
    std::unique_ptr<RGravity> gravity;

    PSystemGravity(PSystemDefinition& def, std::unique_ptr<RGravity> gravity);
    virtual void AddParticles(PManager* manager);
};