#include "systemGravity.h"

#include <tracy/Tracy.hpp>

PSystemGravity::PSystemGravity(PSystemDefinition& def, std::unique_ptr<RGravity> gravity) : PSystem(def, glm::vec3(0.0f), nullptr)
{
    ZoneScopedN("PSystemGravity::PSystemGravity");

    this->gravity = std::move(gravity);
}

void PSystemGravity::AddParticles(PManager* manager)
{
    ZoneScopedN("PSystemGravity::AddParticles");

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
        particles.emplace_back(this, i, numToSpawn);

        // Hijack particle velocity and position.
        PParticle& particle = particles.back();
        particle.pos = gravity->RandomInRange();
        glm::vec3 gravityDir;
        if (gravity->CalcGravity(particle.pos, &gravityDir, nullptr))
        {
            particle.vel = gravityDir * info.speed;
        }
        else
        {
            particles.pop_back();
        }
    }

}