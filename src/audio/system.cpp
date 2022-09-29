#include "system.h"
#include "raudio/raudio.h"

#include <tracy/Tracy.hpp>

void ASystem_Init()
{
    ZoneScopedN("ASystem_Init");
    InitAudioDevice();
}

void ASystem_Close()
{
    ZoneScopedN("ASystem_Close");
    CloseAudioDevice();
}