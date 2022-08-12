#include "jitter.h"

void PEffectJitter::Process(PParticle& particle, glm::vec3& vel)
{
    bool inPeriod = false;
    if (JFrame::currentFrame - lastPeriod > period)
    {
        lastPeriod = JFrame::currentFrame;
        inPeriod = true;
    }
    if (inPeriod == 0)
    {
        vel.x += mag.x * (int)((unsigned int)rand() / 0x800000 - 0x100) / 0x100;
        vel.y += mag.y * (int)((unsigned int)rand() / 0x800000 - 0x100) / 0x100;
        vel.z += mag.z * (int)((unsigned int)rand() / 0x800000 - 0x100) / 0x100;
    }
}