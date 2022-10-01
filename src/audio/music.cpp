#include "music.h"

#include "../fs.h"
#include <tracy/Tracy.hpp>

AMusic::AMusic(std::string path, unsigned int loopStart, unsigned int loopEnd, float volume, float pan, float pitch)
{
    ZoneScopedN("AMusic::AMusic");

    music = LoadMusicStream(FPath::RelPath(path).c_str());
    SetMusicVolume(music, volume);
    SetMusicPan(music, pan);
    SetMusicPitch(music, pitch);
    music.loopStart = loopStart;
    music.loopEnd = (loopEnd == 0) ? music.frameCount : loopEnd;
}

void AMusic::SetVol(float newVolume)
{
    ZoneScopedN("AMusic::SetVol");

    SetMusicVolume(music, newVolume);
}

void AMusic::SetPan(float newPan)
{
    ZoneScopedN("AMusic::SetPan");

    SetMusicPan(music, newPan);
}

void AMusic::SetPitch(float newPitch)
{
    ZoneScopedN("AMusic::SetPitch");

    SetMusicPitch(music, newPitch);
}

void AMusic::SetPos(float newPos)
{
    ZoneScopedN("AMusic::SetPos");

    SeekMusicStream(music, newPos);
}

float AMusic::GetPos()
{
    ZoneScopedN("AMusic::GetPos");

    return GetMusicTimePlayed(music);
}

float AMusic::GetLen()
{
    ZoneScopedN("AMusic::GetLen");

    return GetMusicTimeLength(music);
}

bool AMusic::Playing()
{
    ZoneScopedN("AMusic::Playing");

    return IsMusicStreamPlaying(music);
}

void AMusic::Play()
{
    ZoneScopedN("AMusic::Play");

    if (!paused) PlayMusicStream(music);
    else ResumeMusicStream(music);
    paused = false;
}

void AMusic::Pause()
{
    ZoneScopedN("AMusic::Pause");

    PauseMusicStream(music);
    paused = true;
}

void AMusic::Stop()
{
    ZoneScopedN("AMusic::Stop");

    StopMusicStream(music);
    paused = false;
}

void AMusic::Update()
{
    ZoneScopedN("AMusic::Update");

    UpdateMusicStream(music);
}

AMusic::~AMusic()
{
    ZoneScopedN("AMusic::~AMusic");

    Stop();
    UnloadMusicStream(music);
}

void AMusicTracked::AddTrack(std::string path, float volume, float pan, float pitch)
{
    ZoneScopedN("AMusicTracked::AddTrack");

    tracks.push_back(std::make_unique<AMusic>(path, loopStart, loopEnd, volume, pan, pitch));
}

AMusic* AMusicTracked::GetTrack(int index)
{
    ZoneScopedN("AMusicTracked::GetTrack");

    if (index >= tracks.size()) return nullptr;
    else return tracks[index].get();
}

void AMusicTracked::Play()
{
    ZoneScopedN("AMusicTracked::Play");

    for (unsigned int i = 0; i < tracks.size(); i++)
    {
        tracks[i]->Play();
    }
}

void AMusicTracked::Pause()
{
    ZoneScopedN("AMusicTracked::Pause");

    for (unsigned int i = 0; i < tracks.size(); i++)
    {
        tracks[i]->Pause();
    }
}

void AMusicTracked::Stop()
{
    ZoneScopedN("AMusicTracked::Stop");

    for (unsigned int i = 0; i < tracks.size(); i++)
    {
        tracks[i]->Stop();
    }
}

void AMusicTracked::Update()
{
    ZoneScopedN("AMusicTracked::Update");

    for (unsigned int i = 0; i < tracks.size(); i++)
    {
        tracks[i]->Update();
    }
}