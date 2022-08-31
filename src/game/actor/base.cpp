#include "base.h"
#include "../scenario.h"

void GActorBase::Kill(unsigned int i)
{
    CleanupResources();
    scenario.actors.erase(scenario.actors.begin() + i);
}