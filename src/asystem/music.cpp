#include "music.h"

AMusic::AMusic(std::string path, unsigned int loopStart, unsigned int loopEnd, float volume, float pan, float pitch)
{
    music = LoadMusicStream(path.c_str());
    SetMusicVolume(music, volume);
    SetMusicPan(music, pan);
    SetMusicPitch(music, pitch);
    music.loopStart = loopStart;
    music.loopEnd = (loopEnd == 0) ? music.frameCount : loopEnd;
}

void AMusic::SetVol(float newVolume)
{
    SetMusicVolume(music, newVolume);
}

void AMusic::SetPan(float newPan)
{
    SetMusicPan(music, newPan);
}

void AMusic::SetPitch(float newPitch)
{
    SetMusicPitch(music, newPitch);
}

void AMusic::SetPos(float newPos)
{
    SeekMusicStream(music, newPos);
}

float AMusic::GetPos()
{
    return GetMusicTimePlayed(music);
}

float AMusic::GetLen()
{
    return GetMusicTimeLength(music);
}

bool AMusic::Playing()
{
    return IsMusicStreamPlaying(music);
}

void AMusic::Play()
{
    if (!paused) PlayMusicStream(music);
    else ResumeMusicStream(music);
    paused = false;
}

void AMusic::Pause()
{
    PauseMusicStream(music);
    paused = true;
}

void AMusic::Stop()
{
    StopMusicStream(music);
    paused = false;
}

AMusic::~AMusic()
{
    Stop();
    UnloadMusicStream(music);
}

void AMusicTracked::AddTrack(std::string path, float volume = 0.8f, float pan = 0.5f, float pitch = 1.0f)
{
    tracks.push_back(std::make_unique<AMusic>(path, loopStart, loopEnd, volume, pan, pitch));
}

AMusic* AMusicTracked::GetTrack(int index)
{
    if (index >= tracks.size()) return nullptr;
    else return tracks[index].get();
}

void AMusicTracked::Play()
{
    for (unsigned int i = 0; i < tracks.size(); i++)
    {
        tracks[i]->Play();
    }
}

void AMusicTracked::Pause()
{
    for (unsigned int i = 0; i < tracks.size(); i++)
    {
        tracks[i]->Pause();
    }
}

void AMusicTracked::Stop()
{
    for (unsigned int i = 0; i < tracks.size(); i++)
    {
        tracks[i]->Stop();
    }
}