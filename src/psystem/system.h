#pragma once

#include "effect.h"
#include "glitter.h"
#include "spawnInfo.h"
#include "transition.h"
#include <stack>

struct PManager;

// Particle system definition.
struct PSystemDefinition
{
    PSpawnInfo spawnInfo;
    std::vector<std::shared_ptr<PTransition>> transitions;
    std::shared_ptr<PGlitter> glitter = nullptr;
    std::vector<std::shared_ptr<PEffect>> effects;

    // Don't use this, just to get indices in the manager to not complain even though this will never occur.
    PSystemDefinition() {}

    // Load a particle definition.
    PSystemDefinition(PTextureCache& texCache, std::string name);

};

// System.
struct PSystem
{
    PSystemDefinition* definition;
    bool stopped;
    bool spawnPaused;
    bool paused;
    glm::vec3 pos;
    glm::vec3 vel;
    int age;
    int rateTracker;
    glm::vec3 dir;
    glm::vec3 tangent;
    glm::vec3 tangent2;
    float startHorizontalDistance;
    float horizontalSpeed;
    float verticalSpeed;
    float scale;
    unsigned int particleLifetime;
    unsigned int spawnPeriod;
    unsigned int alpha;
    std::vector<PParticle*> particles; // Reference particles from manager.
    std::vector<PParticle*> glitterParticles;

    // Initialize a system.
    PSystem(PSystemDefinition& def, glm::vec3 pos, glm::vec3* dir);

    // Calculate tangents.
    void CalcTangents();

    // Add particles.
    void AddParticles(PManager* manager);

    // Update particles.
    void Update(PManager* manager);

    // Render particles. TODO!!!
    void Render();

};