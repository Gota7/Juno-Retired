#include "jitter.h"

void PEffectJitter::Process(PParticle& particle, glm::vec3& vel)
{
    if (particle.age % period == 0)
    {   
        vel.x += mag.x * (int)((unsigned int)rand() / 0x800000 - 0x100) / 0x100;
        vel.y += mag.y * (int)((unsigned int)rand() / 0x800000 - 0x100) / 0x100;
        vel.z += mag.z * (int)((unsigned int)rand() / 0x800000 - 0x100) / 0x100;
    }
}