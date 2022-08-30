#pragma once

#include "../jsystem.h"
#include "../particle.h"
#include "scenario.h"
#include <map>
#include <memory>
#include <string>

// Game.
struct GGame
{
    std::unique_ptr<GScenario> scenario;
    std::unique_ptr<JCamera> camera;
    std::unique_ptr<JShader> shader;
    std::unique_ptr<JShader> skyboxShader;
    std::unique_ptr<JShader> particleShader;
    std::unique_ptr<JShader> kclShader;
    std::unique_ptr<JLightDirectional> lightDirectional;
    std::unique_ptr<JUniformBuffer> matrices;
    std::unique_ptr<PManager> particleMgr;

    // Initialize the game.
    GGame();

    // Get a scenario list for a level.
    std::map<int, GScenarioEntry> GetScenarioList(std::string level);

    // Load a level scenario.
    void LoadLevelScenario(std::string level, int scenario);

    // Render the game.
    void Render();

};