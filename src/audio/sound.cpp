#include "sound.h"

ASound::ASound(std::string path, float volume, float pan, float pitch)
{
    sound = LoadSound(path.c_str());
    SetSoundVolume(sound, volume);
    SetSoundPan(sound, pan);
    SetSoundPitch(sound, pitch);
}

void ASound::SetVol(float newVolume)
{
    SetSoundVolume(sound, newVolume);
}

void ASound::SetPan(float newPan)
{
    SetSoundPan(sound, newPan);
}

void ASound::SetPitch(float newPitch)
{
    SetSoundPitch(sound, newPitch);
}

void ASound::Play()
{
    PlaySoundMulti(sound);
}

ASound::~ASound()
{
    UnloadSound(sound);
}

bool ASoundCollection::HasSound(std::string sound)
{
    return sounds.find(sound) != sounds.end();
}

void ASoundCollection::LoadSound(std::string sound, std::string path, float volume, float pan, float pitch)
{
    if (!HasSound(sound))
    {
        sounds[sound] = std::make_unique<ASound>(path, volume, pan, pitch);
    }
}

ASound* ASoundCollection::GetSound(std::string sound)
{
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
    if (HasSound(sound))
    {
        sounds.erase(sound);
    }
}

ASoundCollection::~ASoundCollection()
{
    StopSoundMulti();
}