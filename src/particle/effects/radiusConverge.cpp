#include "radiusConverge.h"

#include <tracy/Tracy.hpp>

void PEffectRadiusConverge::Process(PParticle& particle, glm::vec3& vel)
{
    ZoneScopedN("PEffectRadiusConverge::Process");

    particle.offset += mag * (offset - particle.offset);
}