#pragma once

#include "../gravity.h"
#include "../jsystem.h"
#include "../kcl.h"
#include "actor/base.h"
#include "modelGroup.h"
#include "planet.h"
#include "skybox.h"

// Scenario entry.
struct GScenarioEntry
{
    std::string title;
    std::string yaml;
};

// Fog type.
enum GScenarioFogType
{
    FOG_LINEAR,
    FOG_EXP,
    FOG_EXP2
};

// Scenario fog.
struct GScenarioFog
{
    glm::vec3 color;
    float start;
    float end;
    float density;
    GScenarioFogType type;

    // Set environment fog.
    void Set(JShader& shader);

    // Disable environment fog.
    static void Disable(JShader& shader);

};

// Collider info.
struct GScenarioCollider
{
    std::string type;
    std::string path;
    glm::mat4 mat;
    bool debug;
};

// Actual layout of a level.
struct GScenario
{
    GModelGroup modelGroup; // Model group.
    JShader& defShader; // Default shader.
    JShader& skyboxShader; // Skybox shader.
    JShader& kclShader; // Collision shader.
    std::unique_ptr<GScenarioFog> fog; // Fog.
    std::unique_ptr<GSkybox> skybox; // Skybox.
    std::vector<std::unique_ptr<GPlanet>> planets; // Planets.
    std::vector<std::unique_ptr<JModel>> miscModels; // Models.
    KTree colliders; // For collisions.
    std::vector<GScenarioCollider> colliderInfo; // For editor since it would be expensive to unravel loaded collisions with small memory hit.
    RManager gravMgr; // Gravity manager.
    std::vector<std::unique_ptr<GActorBase>> actors; // Actors to use.
    bool paused = false; // Pause the scenario.

    // Temporary constructor.
    GScenario(JShader& defShader, JShader& skyboxShader, JShader& kclShader) : defShader(defShader), skyboxShader(skyboxShader), kclShader(kclShader), colliders(std::vector<std::unique_ptr<KMesh>>()) {}

    // Load a scenario.
    void Load(std::string yaml);

    // Save a scenario.
    void Save(std::string yaml);

    // Render the scenario.
    void Render();

    // Update the scenario.
    void Update();

};