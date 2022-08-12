#pragma once

#include "particle.h"
#include "system.h"
#include <map>
#include <vector>

// Manager for spawning and handling particles.
struct PManager
{
    std::vector<PSystem> systems; // Vector approach is efficient since memory isn't freed until vector is deleted. It is reused.
    std::vector<PParticle> particles; // Same as above, doesn't make sense to have in individual systems.
    PTextureCache textureCache; // Texture cache.
    std::map<std::string, PSystemDefinition> definitions; // Loaded system definitions.

    // Construct a new particle manager.
    PManager(int systemsToReserve, int particlesToReserve);

    // Add a system by string.
    PSystem& AddSystem(std::string name, glm::vec3 pos, glm::vec3* dir);

    // Add a system.
    PSystem& AddSystem(PSystemDefinition& def, glm::vec3 pos, glm::vec3* dir);

    // Update the system.
    void Update();

    // Render the system. TODO!!!
    void Render();

};