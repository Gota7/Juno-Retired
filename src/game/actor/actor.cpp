#include "actor.h"
#include "../scenario.h"

bool GActor::Update()
{
    if (!BeforeUpdate()) return false;
    scenario.gravMgr.CalcGravity(body.pos, GRAVITY_NORMAL, &grav);
    body.Update(scenario.colliders, grav, (float)JFrame::deltaTime);
    if (!AfterUpdate()) return false;
    return true;
}

bool GActor::VerifyArgs(const GSpawnInfo& info, unsigned int minVecs, unsigned int minFloats, unsigned int minStrings)
{
    if (info.vecs.size() < minVecs) { std::cout << "ERROR: Initializing " + info.actorID + " failed. Expected at least " + std::to_string(minVecs) + " vector args but got " + std::to_string(info.vecs.size()) + "." << std::endl; return false; }
    if (info.floats.size() < minFloats) { std::cout << "ERROR: Initializing " + info.actorID + " failed. Expected at least " + std::to_string(minFloats) + " float args but got " + std::to_string(info.floats.size()) + "." << std::endl; return false; }
    if (info.strings.size() < minStrings) { std::cout << "ERROR: Initializing " + info.actorID + " failed. Expected at least " + std::to_string(minStrings) + " string args but got " + std::to_string(info.strings.size()) + "." << std::endl; return false; }
    return true;
}