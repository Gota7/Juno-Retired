#include "system.h"

#include "manager.h"
#include <yaml-cpp/yaml.h>

PSystemDefinition::PSystemDefinition(PTextureCache& texCache, std::string name)
{
    YAML::Node root = YAML::LoadFile("res/pcl/Definitions/" + name + ".yaml");
    spawnInfo.Load(texCache, root["SpawnInfo"].as<std::string>());
}

PSystem::PSystem(PSystemDefinition& def, glm::vec3 pos, glm::vec3* dir)
{
    particles.reserve(32); // Probably fine for now don't @me.
    glitterParticles.reserve(16);
    definition = &def;
    stopped = false;
    spawnPaused = false;
    paused = false;
    this->pos = pos;
    vel = glm::vec3(0.0f);
    age = 0;
    rateTracker = 0;
    this->dir = dir ? (*dir) : def.spawnInfo.dir;
    startHorizontalDistance = def.spawnInfo.maxDist;
    horizontalSpeed = def.spawnInfo.perpSpeed;
    verticalSpeed = def.spawnInfo.speed;
    scale = def.spawnInfo.scale;
    particleLifetime = def.spawnInfo.lifetime;
    spawnPeriod = def.spawnInfo.spawnPeriod;
    alpha = def.spawnInfo.alpha;
}

void PSystem::CalcTangents()
{
    glm::vec3 vec = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 normalDir = glm::normalize(dir);
    if (glm::abs(normalDir.x * 0x1000) == 0
            && glm::abs(normalDir.y * 0x1000) == 0x1000
            && glm::abs(normalDir.z * 0x1000) == 0)
        vec = glm::vec3(1.0f, 0.0f, 0.0f);
    tangent = glm::cross(normalDir, vec);
    tangent2 = glm::cross(normalDir, tangent);
}

void PSystem::AddParticles(PManager* manager)
{

    // Get spawn count.
    PSpawnInfo& info = definition->spawnInfo;
    int numToSpawn = rateTracker + (int)(info.spawnRate * 0x1000);
    rateTracker = numToSpawn & 0xFFF;
    numToSpawn /= 0x1000;

    // Calc tangents if needed.
    if ((info.spawnShape == P_SPAWNSHAPE_CIRCLE_EVEN
            || info.spawnShape == P_SPAWNSHAPE_CIRCLE_RANDOM
            || info.spawnShape == P_SPAWNSHAPE_DISC)
            && info.plane == P_PARTICLEPLANE_DIR)
        CalcTangents();

    // Finally spawn particles.
    for (int i = 0; i < numToSpawn; i++)
    {
        PParticle& particle = manager->particles.emplace_back(this, i, numToSpawn);
        particles.emplace_back(&particle);
    }

}

void PSystem::Update(PManager* manager)
{
    PSpawnInfo& info = definition->spawnInfo;
    if ((info.frames == 0 || age < info.frames)
        && age % spawnPeriod == 0 && !stopped && !spawnPaused)
        AddParticles(manager);

    // TODO: HANDLE TRANSITIONS!!!

    // Update particles.
    for (int i = particles.size() - 1; i >= 0; i--)
    {
        PParticle* particle = particles[i];

        // TODO: PROCESS TRANSITIONS!!!

        // Update particle.
        particle->Update(this);
        if (particle->age > particle->lifetime)
        {
            particles.erase(particles.begin() + i);
            particle->die = true;
        }
    }

    // TODO: HANDLE GLITTER!!!

    // Increment age.
    age++;

}

void PSystem::Render()
{
}