#pragma once

#include "../jsystem.h"
#include <memory>

struct GScenario;

// Planet.
struct GPlanet
{
    std::shared_ptr<JModel> model; // Normal size model.
    std::shared_ptr<JModel> modelLow; // Low poly model.
    std::string modelPath; // Path to normal size model.
    std::string modelLowPath; // Path to low poly model.
    glm::mat4 mat; // Transformation matrix.
    constexpr static float MAX_DIST_TO_PLAYER = 100.0f;

    // Make a new planet.
    GPlanet(GScenario* scenario, std::string model, std::string modelLow, glm::mat4 mat);

    // Render a planet.
    void Render(float distToPlayer);

};