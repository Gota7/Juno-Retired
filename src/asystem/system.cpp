#include "system.h"
#include "raudio/raudio.h"

void ASystem_Init()
{
    InitAudioDevice();
}

void ASystem_Close()
{
    CloseAudioDevice();
}