#include "particle.h"

#include "manager.h"
#include "system.h"
#include <tracy/Tracy.hpp>

glm::vec3 RandomNormalizedXY()
{
    ZoneScopedN("RandomNormalizedXY");

    return glm::normalize(glm::vec3(JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f), 0));
}

glm::vec3 RandomNormalized()
{
    ZoneScopedN("RandomNormalized");

    return glm::normalize(glm::vec3(JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f), JRandom::RandomInRange(-1.0f, 1.0f)));
}

PParticle::PParticle(PSystem* system, int index, int total) : sprite(system->definition->spawnInfo.texture)
{
    ZoneScopedN("PParticle::PParticle");

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
    float randHorizontalSpeed = system->horizontalSpeed + ((rand() % 2000) / 1000.0f - 1.0f) * info.speedRandomness;
    float randVerticalSpeed = system->verticalSpeed + ((rand() % 2000) / 1000.0f - 1.0f) * info.speedRandomness;
    glm::vec3 offsetDir = glm::vec3(0.0f);
    if ((int)(offset.x * 0x1000) == 0
        && (int)(offset.y * 0x1000) == 0
        && (int)(offset.z * 0x1000) == 0)
        offsetDir = RandomNormalized();
    else
        offsetDir = glm::normalize(offset);

    vel = system->dir * randVerticalSpeed + offsetDir * randHorizontalSpeed;
    pos = system->pos + offset;

    scale = system->scale + ((rand() % 2000) / 1000.0f - 1.0f) * info.scaleRandomness;
    scaleMult = 1.0f;

    // TODO: COLOR TRANSITION STUFF!
    color = info.color;
    alpha = system->alpha;
    alphaMult = 0x1F;

    angle = info.randomInitAngle ? glm::radians((float)(rand() % 360)) : (float)0;
    // TODO: RANDOM ANGLE SPEED IF ROTATING!
    angleSpeed = 0;

    lifetime = system->particleLifetime + ((rand() % 2000) / 1000.0f - 1.0f) * info.lifetimeRandomness;
    age = 0;

    // TODO: TEXTURE SEQUENCE!
    //sprite = info.texture;

    altLenInv = 0x10000 / (info.altLength != 0 ? info.altLength : 1);
    lifetimeInv = 0x10000 / (lifetime != 0 ? lifetime : 1);
}

void PParticle::Update(PSystem* system)
{
    ZoneScopedN("PParticle::Update");

    PSpawnInfo& info = system->definition->spawnInfo;
    if (info.followSystem) pos = system->pos + offset;

    glm::vec3 velInc = glm::vec3(0.0f);
    // TODO: EFFECTS!!!

    angle += angleSpeed;
    //vel = vel * (float)(info.speedFalloff + 0x180) / (float)0x200 + velInc;
    offset += vel * system->vel;
    pos += vel;

    // TODO: GLITTER!!!

    age += (float)JFrame::deltaTime;
}

void PParticle::Render(PManager* manager, JShader& shader, bool glitter)
{
    ZoneScopedN("PParticle::Render");

    glm::mat4 mat = glm::scale(glm::translate(glm::mat4(1.0), pos), glm::vec3(1.0, 1.0, 1.0) * scale);
    shader.SetMatrix("model", glm::value_ptr(mat));
    shader.SetVec4("color", glm::value_ptr(glm::vec4(color, alpha)));
    manager->mesh->Render();
}