#include "limitPlane.h"

#include <tracy/Tracy.hpp>

void PEffectLimitPlane::Process(PParticle& particle, glm::vec3& vel)
{
    ZoneScopedN("PEffectLimitPlane::Process");

    if (behaviour == PEFFECT_LIMITPLANE_CHANGE_SPEED)
    {
        if (particle.pos.y < posY && posY < particle.pos.y + particle.offset.y)
        {
            particle.offset.y = posY - particle.pos.y;
            particle.vel.y *= -reverseSpeedMult;
        }
        else if (particle.pos.y > posY && particle.pos.y + particle.offset.y < posY)
        {
            particle.offset.y = posY - particle.pos.y;
            particle.vel.y *= -reverseSpeedMult;
        }
    }
    else if (behaviour == PEFFECT_LIMITPLANE_KILL)
    {
        if (particle.pos.y < posY && posY < particle.pos.y + particle.offset.y)
            particle.age = particle.lifetime;
        else if (posY < particle.pos.y && particle.pos.y + particle.offset.y < posY)
            particle.age = particle.lifetime;
    }
}