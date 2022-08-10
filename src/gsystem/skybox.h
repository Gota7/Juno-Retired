#pragma once

#include "../jsystem.h"

// Forward declarations.
struct GScenario;

// Standard skybox.
struct GSkybox
{
    std::shared_ptr<JModel> model; // Standard skybox model.

    // Make a new skybox.
    GSkybox(GScenario* scenario, ModelCubemapTextures textures);

    // Render the skybox.
    void Render();

};