#include "manager.h"

PManager::PManager(int systemsToReserve, int particlesToReserve)
{
    systems.reserve(systemsToReserve);
    particles.reserve(particlesToReserve);
}

PSystem& PManager::AddSystem(std::string name, glm::vec3 pos, glm::vec3* dir)
{
    if (definitions.find(name) == definitions.end())
    {
        definitions[name] = PSystemDefinition(textureCache, name);
    }
    return AddSystem(definitions[name], pos, dir);
}

PSystem& PManager::AddSystem(PSystemDefinition& def, glm::vec3 pos, glm::vec3* dir)
{
    return systems.emplace_back(def, pos, dir);
}

void PManager::Update()
{
    for (int i = particles.size() - 1; i >= 0; i--)
    {
        if (particles[i].die) particles.erase(particles.begin() + i);
    }
    for (int i = systems.size() - 1; i >= 0; i--)
    {
        PSystem& sys = systems[i];
        if (!sys.paused) sys.Update(this);
        PSpawnInfo& info = sys.definition->spawnInfo;
        if ((info.frames != 0 && sys.age > info.frames || sys.stopped)
            && sys.particles.size() == 0 && sys.glitterParticles.size() == 0)
        {
            sys.stopped = true;
            systems.erase(systems.begin() + i);
        }
    }
}

void PManager::Render()
{
}