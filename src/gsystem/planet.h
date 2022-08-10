#pragma once

#include "../jsystem.h"
#include "scenario.h"
#include <memory>

// Planet.
struct GPlanet
{
    std::shared_ptr<JModel> model; // Normal size model.
    std::shared_ptr<JModel> modelLow; // Low poly model.
    glm::mat4 mat; // Transformation matrix.
    constexpr static float MAX_DIST_TO_PLAYER = 100.0f;

    // Make a new planet.
    GPlanet(GScenario* scenario, std::string model, std::string modelLow, glm::mat4 mat);

    // Render a planet.
    void Render(float distToPlayer);

};