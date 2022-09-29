#include "converge.h"

#include <tracy/Tracy.hpp>

void PEffectConverge::Process(PParticle& particle, glm::vec3& vel)
{
    ZoneScopedN("PEffectConverge::Process");

    vel += mag * (offset - particle.offset - particle.vel);
}