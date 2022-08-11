#include "acceleration.h"

void PEffectAcceleration::Process(PParticle& particle, glm::vec3& vel)
{
    vel += accel;
}