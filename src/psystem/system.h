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
    std::vector<PParticle*> particles;
    std::vector<PParticle*> glitterParticles;

    // Initialize a system.
    void Init(PSystemDefinition& def, glm::vec3 pos, glm::vec3* dir);

    // Calculate tangents.
    void CalcTangents();

    // Add particles.
    void AddParticles(std::stack<PParticle*>& freeParticles);

    // Update particles.
    void Update(PManager* manager);

    // Render particles. TODO!!!
    void Render();

};