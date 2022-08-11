#pragma once

#include "particle.h"
#include "system.h"
#include <stack>
#include <vector>

// Manager for spawning and handling particles.
struct PManager
{
    int numParticles;
    int numSystems;
    std::stack<PParticle*> freeParticles;
    std::stack<PSystem*> freeSystems;
    std::unique_ptr<PParticle[]> allocatedParticles;
    std::unique_ptr<PSystem[]> allocatedSystems;
    std::vector<PSystem*> systemsInUse;

    // Construct a new particle manager.
    PManager(int numParticles, int numSystems);

    // Add a system.
    PSystem* AddSystem(PSystemDefinition& def, glm::vec3 pos, glm::vec3* dir);

    // Update the system.
    void Update();

    // Render the system. TODO!!!
    void Render();

};