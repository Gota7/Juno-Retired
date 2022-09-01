#include "actor.h"
#include "../scenario.h"

bool GActor::Update()
{
    if (!BeforeUpdate()) return false;
    glm::vec3 grav(0.0f, -1.0f, 0.0f);
    scenario.gravMgr.CalcGravity(body.pos, GRAVITY_NORMAL, &grav);
    body.Update(scenario.colliders, grav, (float)JFrame::deltaTime);
    if (!AfterUpdate()) return false;
    return true;
}