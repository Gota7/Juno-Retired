#include "sound.h"

#include <tracy/Tracy.hpp>

ASound::ASound(std::string path, float volume, float pan, float pitch)
{
    ZoneScopedN("ASound::ASound");

    sound = LoadSound(path.c_str());
    SetSoundVolume(sound, volume);
    SetSoundPan(sound, pan);
    SetSoundPitch(sound, pitch);
}

void ASound::SetVol(float newVolume)
{
    ZoneScopedN("ASound::SetVol");

    SetSoundVolume(sound, newVolume);
}

void ASound::SetPan(float newPan)
{
    ZoneScopedN("ASound::SetPan");

    SetSoundPan(sound, newPan);
}

void ASound::SetPitch(float newPitch)
{
    ZoneScopedN("ASound::SetPitch");

    SetSoundPitch(sound, newPitch);
}

void ASound::Play()
{
    ZoneScopedN("ASound::Play");

    PlaySoundMulti(sound);
}

ASound::~ASound()
{
    ZoneScopedN("ASound::~ASound");

    UnloadSound(sound);
}

bool ASoundCollection::HasSound(std::string sound)
{
    ZoneScopedN("ASoundCollection::HasSound");

    return sounds.find(sound) != sounds.end();
}

void ASoundCollection::LoadSound(std::string sound, std::string path, float volume, float pan, float pitch)
{
    ZoneScopedN("ASoundCollection::LoadSound");

    if (!HasSound(sound))
    {
        sounds[sound] = std::make_unique<ASound>(path, volume, pan, pitch);
    }
}

ASound* ASoundCollection::GetSound(std::string sound)
{
    ZoneScopedN("ASoundCollection::GetSound");

    if (HasSound(sound))
    {
        return sounds[sound].get();
    }
    else
    {
        return nullptr;
    }
}

void ASoundCollection::UnloadSound(std::string sound)
{
    ZoneScopedN("ASoundCollection::UnloadSound");

    if (HasSound(sound))
    {
        sounds.erase(sound);
    }
}

ASoundCollection::~ASoundCollection()
{
    ZoneScopedN("ASoundCollection::~ASoundCollection");
    StopSoundMulti();
}