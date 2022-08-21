#include "radiusConverge.h"

void PEffectRadiusConverge::Process(PParticle& particle, glm::vec3& vel)
{
    particle.offset += mag * (offset - particle.offset);
}