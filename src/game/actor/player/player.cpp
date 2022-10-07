#include "player.h"
#include "../../game.h"
#include "../../scenario.h"

#include <tracy/Tracy.hpp>

GActorPlayer::GActorPlayer(GScenario& scenario, std::string actorID, glm::vec3 pos) : GActor(scenario, actorID, 1.0f)
{
    ZoneScopedN("GActorPlayer::GActorPlayer");

    body.pos = pos;
}

// Taken from Pants64DS. He's really good with math so this should be reliable.
void GActorPlayer::UpdatePlayerMovementAxes()
{
    ZoneScopedN("GActorPlayer::UpdatePlayerMovementAxes");

    glm::vec3 xc = glm::normalize(glm::cross(scenario.game.camera->cameraFront, scenario.game.camera->cameraUp));
    glm::vec3 yc = scenario.game.camera->cameraUp;
    glm::vec3 y = -grav;
    glm::vec3 u = glm::normalize(glm::cross(yc, y));
    glm::vec3 v = glm::cross(xc, y);
    playerZAxis = v - u * (glm::sign(glm::dot(y, xc)) * glm::sqrt(KUtil::Square(glm::dot(v, u)) - glm::dot(v, v) + 1) + glm::dot(v, u));
    playerXAxis = glm::cross(y, playerZAxis);
}

bool GActorPlayer::UpdateCamera()
{
    ZoneScopedN("GActorPlayer::UpdateCamera");

    return false;
}

bool GActorPlayer::LoadResources()
{
    ZoneScopedN("GActorPlayer::LoadResources");

    system = &scenario.game.particleMgr->AddSystem("LameSphere", body.pos, nullptr);
    return true;
}

bool GActorPlayer::BeforeUpdate()
{
    ZoneScopedN("GActorPlayer::BeforeUpdate");

    //body.vel = grav * 0.5f + glm::vec3(0.18f, 0.0f, -0.18f); // Set constant speed to gravity and more.
    body.vel = grav * 0.5f;
    system->pos = body.pos;
    return true;
}

bool GActorPlayer::AfterUpdate()
{
    scenario.game.camera->needsMatrixUpdate |= UpdateCamera();
    return true;
}

GActorPlayer::~GActorPlayer()
{
    ZoneScopedN("GActorPlayer::~GActorPlayer");

    system->stopped = true;
}