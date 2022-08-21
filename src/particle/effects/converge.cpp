#include "converge.h"

void PEffectConverge::Process(PParticle& particle, glm::vec3& vel)
{
    vel += mag * (offset - particle.offset - particle.vel);
}