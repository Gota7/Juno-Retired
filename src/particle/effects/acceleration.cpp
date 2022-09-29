#include "acceleration.h"

#include <tracy/Tracy.hpp>

void PEffectAcceleration::Process(PParticle& particle, glm::vec3& vel)
{
    ZoneScopedN("PEffectAcceleration::Process");

    vel += accel;
}