#pragma once

#include "../input.h"
#include "../jsystem.h"
#include "../particle.h"
#include "cameras/followCam.h"
#include "scenario.h"
#include <map>
#include <memory>
#include <string>

// Game.
struct GGame
{
    std::unique_ptr<GScenario> scenario;
    std::unique_ptr<GFollowCam> camera;
    std::unique_ptr<JShader> shader;
    std::unique_ptr<JShader> skyboxShader;
    std::unique_ptr<JShader> particleShader;
    std::unique_ptr<JShader> kclShader;
    std::unique_ptr<JLightDirectional> lightDirectional;
    std::unique_ptr<JUniformBuffer> matrices;
    std::unique_ptr<PManager> particleMgr;
    std::unique_ptr<IController> input;

    // Initialize the game.
    GGame(GLFWwindow* window);

    // Get a scenario list for a level.
    std::map<int, GScenarioEntry> GetScenarioList(std::string level);

    // Load a level scenario.
    void LoadLevelScenario(std::string level, int scenario);

    // Render the game.
    void Render();

};