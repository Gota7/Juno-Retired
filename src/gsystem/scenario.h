#pragma once

#include "../jsystem.h"
#include "modelGroup.h"
#include "planet.h"
#include "skybox.h"

// Scenario entry.
struct GScenarioEntry
{
    std::string title;
    std::string yaml;
};

// Actual layout of a level.
struct GScenario
{
    GModelGroup modelGroup; // Model group.
    JShader& defShader; // Default shader.
    JShader& skyboxShader; // Skybox shader.
    std::unique_ptr<GSkybox> skybox; // Skybox.
    std::vector<std::unique_ptr<GPlanet>> planets; // Planets.

    // Temporary constructor.
    GScenario(JShader& defShader, JShader& skyboxShader) : defShader(defShader), skyboxShader(skyboxShader) {}

    // Load a scenario.
    void Load(std::string yaml);

    // Save a scenario.
    void Save(std::string yaml);

    // Render the scenario.
    void Render();

};