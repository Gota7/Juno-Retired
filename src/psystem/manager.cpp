#include "manager.h"

PManager::PManager(int numParticles, int numSystems) : numParticles(numParticles), numSystems(numSystems)
{
    allocatedSystems.reset(new PSystem[numSystems]);
    allocatedParticles.reset(new PParticle[numSystems]);
    for (int i = 0; i < numParticles; i++)
    {
        freeParticles.push(&allocatedParticles.get()[i]);
    }
    for (int i = 0; i < numSystems; i++)
    {
        freeSystems.push(&allocatedSystems.get()[i]);
    }
}

PSystem* PManager::AddSystem(PSystemDefinition& def, glm::vec3 pos, glm::vec3* dir)
{
    if (freeSystems.size() == 0) return nullptr;
    PSystem* sys = freeSystems.top();
    freeSystems.pop();
    systemsInUse.push_back(sys);
    sys->Init(def, pos, dir);
    return sys;
}

void PManager::Update()
{
    for (int i = systemsInUse.size() - 1; i >= 0; i--)
    {
        PSystem* sys = systemsInUse[i];
        if (!sys->paused) sys->Update(this);
        PSpawnInfo& info = sys->definition->spawnInfo;
        if ((info.frames != 0 && sys->age > info.frames || sys->stopped)
            && sys->particles.size() == 0 && sys->glitterParticles.size() == 0)
        {
            sys->stopped = true;
            freeSystems.push(sys);
            systemsInUse.erase(systemsInUse.begin() + i);
        }
    }
}

void PManager::Render()
{
}