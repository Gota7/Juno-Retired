#pragma once

#include "raudio/raudio.h"
#include <map>
#include <memory>
#include <string>

// Standard sound.
struct ASound
{
    Sound sound;

    ASound(std::string path, float volume = 0.8f, float pan = 0.5f, float pitch = 1.0f);
    void SetVol(float newVolume = 0.8f);
    void SetPan(float newPan = 0.5f);
    void SetPitch(float newPitch = 1.0f);
    void Play();
    ~ASound();
};

// Collection of sounds (for a level or so).
struct ASoundCollection
{
    std::map<std::string, std::unique_ptr<ASound>> sounds;

    bool HasSound(std::string sound);
    void LoadSound(std::string sound, std::string path, float volume = 0.8f, float pan = 0.5f, float pitch = 1.0f);
    ASound* GetSound(std::string sound);
    void UnloadSound(std::string sound);
    ~ASoundCollection(); // Makes the assumption we are only swapping sound archives around during loading screens when no SFX plays.
};