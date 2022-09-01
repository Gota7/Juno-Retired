#pragma once

#include "../../jsystem.h"

struct GScenario;

// Actor spawn info.
struct GSpawnInfo
{
    GScenario& scenario; // Scenario.
    std::string actorID; // Actor to spawn.
    std::vector<glm::vec3> vecs; // Vector parameters.
    std::vector<float> floats; // Float parameters.
    std::vector<std::string> strings; // String parameters.

    // General constructor.
    GSpawnInfo(GScenario& scenario) : scenario(scenario) {}

};