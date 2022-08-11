#include "particle.h"

#include "system.h"

glm::vec3 RandomNormalizedXY()
{
    return glm::normalize(glm::vec3(rand() >> 8, rand() >> 8, 0));
}

glm::vec3 RandomNormalized()
{
    return glm::normalize(glm::vec3(rand() >> 8, rand() >> 8, rand() >> 8));
}

void PParticle::Init(PSystem* system, int index, int total)
{
    PSpawnInfo& info = system->definition->spawnInfo;
    switch (info.spawnShape)
    {
        case P_SPAWNSHAPE_POINT:
            offset = glm::vec3(0.0f);
            break;
        case P_SPAWNSHAPE_SPHERE:
            offset = RandomNormalized() * system->startHorizontalDistance;
            break;
        case P_SPAWNSHAPE_CIRCLE_RANDOM:
            //SetOffsetOnPlane(system, RandomNormalizedXY() * system->startHorizontalDistance);
            break;
        // TODO: REST OF SHAPES!!!
        default:
            break;
    }
    float randHorizontalSpeed = system->horizontalSpeed * (info.speedRandomness + 0xFF - (int)(info.speedRandomness * ((unsigned int)rand() >> 24) >> 7)) / 0x100;
    float randVerticalSpeed = system->verticalSpeed * (info.speedRandomness + 0xFF - (int)(info.speedRandomness * ((unsigned int)rand() >> 24) >> 7)) / 0x100;
    glm::vec3 offsetDir;
    if ((int)(offset.x * 0x1000) == 0
        && (int)(offset.y * 0x1000) == 0
        && (int)(offset.z * 0x1000) == 0)
        offsetDir = RandomNormalized();
    else
        offsetDir = glm::normalize(offset);

    vel = system->dir * randVerticalSpeed + offsetDir * randHorizontalSpeed;
    pos = system->pos;

    scale = system->scale * (info.scaleRandomness + 0xFF - (int)(info.scaleRandomness * ((unsigned int)rand() >> 24) >> 7)) / 0x100;
    scaleMult = 1.0f;

    // TODO: COLOR TRANSITION STUFF!
    color = info.color;
    alpha = system->alpha;
    alphaMult = 0x1F;

    angle = info.randomInitAngle ? glm::radians((float)(rand() % 360)) : (float)0;
    // TODO: RANDOM ANGLE SPEED IF ROTATING!
    angleSpeed = 0;

    lifetime = (system->particleLifetime * (0xFF - ((int)(info.lifetimeRandomness * ((unsigned int)rand() >> 24)) >> 8)) >> 8) + 1;
    age = 0;

    // TODO: TEXTURE SEQUENCE!
    sprite = info.texture;

    altLenInv = 0x10000 / (info.altLength != 0 ? info.altLength : 1);
    lifetimeInv = 0x10000 / (lifetime != 0 ? lifetime : 1);
}

void PParticle::Update(PSystem* system, std::stack<PParticle*>& freeParticles)
{
}