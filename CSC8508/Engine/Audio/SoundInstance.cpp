#pragma once
#include"SoundInstance.h"

using namespace NCL::CSC8508::Audio;

SoundInstance::SoundInstance():
	sound(nullptr), audioCore(nullptr), mode(FMOD_3D), channelID(-1), volume(1), pitch(1), path("")
{
	attributes3D.pos = ToFMODVECTOR(Maths::Vector3(0,0,0));
	attributes3D.vel = ToFMODVECTOR(Maths::Vector3(0,0,0));
	distances.min = 0;
	distances.max = 0;
}

SoundInstance::~SoundInstance()
{
	//this->Stop();
	SoundManager::DeleteInstance(this);
}



int SoundInstance::Play()
{
	int ChannelID = audioCore->coreNextChannelID++;
	FMOD::Channel* channel = nullptr;

	SoundManager::ErrorCheck(audioCore->coreSystem->playSound(sound, 0, true, &channel));
	if (channel)
	{
		if (mode & MODE_3D)
		{
			SoundManager::ErrorCheck(channel->set3DAttributes(&attributes3D.pos, &attributes3D.vel));
			SoundManager::ErrorCheck(channel->setVolume(volume));
			SoundManager::ErrorCheck(channel->setPitch(pitch));
		}
		SoundManager::ErrorCheck(channel->setPaused(false));
		audioCore->coreChannels[ChannelID] = channel;
	}
	channelID = ChannelID;
	return ChannelID;
}

void SoundInstance::Stop()
{
	if (!this->isPlaying())
		return;

	auto foundChannel = audioCore->coreChannels.find(channelID);
	SoundManager::ErrorCheck((*foundChannel).second->stop());
}

int SoundInstance::isPlaying()
{
	auto foundChannel = audioCore->coreChannels.find(channelID);
	if (foundChannel == audioCore->coreChannels.end())
		return false;
	bool playingState;
	SoundManager::ErrorCheck((*foundChannel).second->isPlaying(&playingState));
	return playingState;
}

void SoundInstance::SetPaused(bool paused)
{
	if (!isPlaying())
		return;

	auto foundChannel = audioCore->coreChannels.find(channelID);
	SoundManager::ErrorCheck(foundChannel->second->setPaused(paused));
}

bool SoundInstance::isPaused()
{
	if (!isPlaying())
		return false;

	bool pauseState;
	auto foundChannel = audioCore->coreChannels.find(channelID);
	SoundManager::ErrorCheck(foundChannel->second->getPaused(&pauseState));
	return pauseState;
}

void SoundInstance::Set3DAttributes(Maths::Vector3 pos, Maths::Vector3 vel)
{
	attributes3D.pos = ToFMODVECTOR(pos);
	attributes3D.vel = ToFMODVECTOR(vel);
}

void SoundInstance::SetMode(Audio_Mode m)
{
	FMOD_MODE fmodMode = FMOD_DEFAULT;
	if (m & Audio_Mode::MODE_LOOP_ON) fmodMode |= FMOD_LOOP_NORMAL;
	if (m & Audio_Mode::MODE_LOOP_OFF) fmodMode |= FMOD_LOOP_OFF;
	if (m & Audio_Mode::MODE_3D) fmodMode |= FMOD_3D;
	if (m & Audio_Mode::MODE_2D) fmodMode |= FMOD_2D;
	SoundManager::ErrorCheck(sound->setMode(fmodMode));
	mode = (int)m;
}

void SoundInstance::SetMode(int m)
{
	FMOD_MODE fmodMode = FMOD_DEFAULT;
	if (m & Audio_Mode::MODE_LOOP_ON) fmodMode |= FMOD_LOOP_NORMAL;
	if (m & Audio_Mode::MODE_LOOP_OFF) fmodMode |= FMOD_LOOP_OFF;
	if (m & Audio_Mode::MODE_3D) fmodMode |= FMOD_3D;
	if (m & Audio_Mode::MODE_2D) fmodMode |= FMOD_2D;
	SoundManager::ErrorCheck(sound->setMode(fmodMode));
	mode = m;
}

void SoundInstance::SetMaxMinDistance(float max, float min)
{
	distances.max = max;
	distances.min = min;
	SoundManager::ErrorCheck(sound->set3DMinMaxDistance(min, max));
}

FMOD_VECTOR SoundInstance::ToFMODVECTOR(const Maths::Vector3& v)
{
	FMOD_VECTOR vec;
	vec.x = v.x;
	vec.y = v.y;
	vec.z = v.z;
	return vec;
}

