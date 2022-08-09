#pragma once

#include "raudio/raudio.h"
#include <memory>
#include <string>
#include <vector>

// Music track.
struct AMusic
{
    Music music;
    bool paused = false;

    AMusic(std::string path, unsigned int loopStart = 0, unsigned int loopEnd = 0, float volume = 0.8f, float pan = 0.5f, float pitch = 1.0f);
    void SetVol(float newVolume = 0.8f);
    void SetPan(float newPan = 0.5f);
    void SetPitch(float newPitch = 1.0f);
    void SetPos(float newPos = 0.0f);
    float GetPos();
    float GetLen();
    bool Playing();
    void Play();
    void Pause();
    void Stop();
    ~AMusic();
};

// Music with multiple tracks.
struct AMusicTracked
{
    std::vector<std::unique_ptr<AMusic>> tracks;
    unsigned int loopStart, loopEnd;

    AMusicTracked(unsigned int loopStart = 0, unsigned int loopEnd = 0) : loopStart(loopStart), loopEnd(loopEnd) {}
    void AddTrack(std::string path, float volume = 0.8f, float pan = 0.5f, float pitch = 1.0f);
    AMusic* GetTrack(int index);
    void Play();
    void Pause();
    void Stop();
};