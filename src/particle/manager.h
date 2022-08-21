#pragma once

#include "../gravity.h"
#include "particle.h"
#include "system.h"
#include "systemGravity.h"
#include <map>
#include <vector>

// Manager for spawning and handling particles.
struct PManager
{
    std::vector<PSystem> systems; // Vector approach is efficient since memory isn't freed until vector is deleted. It is reused.
    std::vector<PSystemGravity> gravitySystems;
    PTextureCache textureCache; // Texture cache.
    std::map<std::string, PSystemDefinition> definitions; // Loaded system definitions.
    std::unique_ptr<JMesh> mesh; // Particle mesh.

    // Construct a new particle manager.
    PManager(int systemsToReserve);

    // Add a system by string.
    PSystem& AddSystem(std::string name, glm::vec3 pos, glm::vec3* dir);

    // Add a system.
    PSystem& AddSystem(PSystemDefinition& def, glm::vec3 pos, glm::vec3* dir);

    // Add a system by gravity type and string.
    PSystemGravity& AddSystemGravity(std::string name, std::unique_ptr<RGravity> gravity);

    // Add a system by gravity type.
    PSystemGravity& AddSystemGravity(PSystemDefinition& def, std::unique_ptr<RGravity> gravity);

    // Update the system.
    void Update();

    // Render the system.
    void Render(JShader& shader);

};