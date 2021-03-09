#pragma once
#include"SoundInstance.h"

using namespace NCL;
using namespace NCL::CSC8508::Audio;

SoundInstance::SoundInstance() :
	sound(nullptr), audioCore(nullptr), isLoop(false), is3D(true), channelID(-1), volume(1), pitch(1), path("")
{
	attributes3D.pos = Audio::ToFMODVECTOR(Maths::Vector3(0,0,0));
	attributes3D.vel = Audio::ToFMODVECTOR(Maths::Vector3(0,0,0));
	distances.min = 10;
	distances.max = 200;
}

SoundInstance::~SoundInstance()
{
	this->Stop();
	SoundManager::DeleteInstance(this);
}



void SoundInstance::Play()
{
	channelID = audioCore->coreNextChannelID++;
	FMOD::Channel* channel = nullptr;

	FMOD_MODE mode = FMOD_DEFAULT;
	is3D ? mode |= FMOD_3D : mode |= FMOD_2D;
	isLoop ? mode |= FMOD_LOOP_NORMAL : mode |= FMOD_LOOP_OFF;

	Audio::ErrorCheck(sound->setMode(mode));
	Audio::ErrorCheck(sound->set3DMinMaxDistance(distances.min, distances.max));
	Audio::ErrorCheck(audioCore->coreSystem->playSound(sound, 0, true, &channel));
	if (channel)
	{
		if (is3D)
		{
			Audio::ErrorCheck(channel->set3DAttributes(&attributes3D.pos, &attributes3D.vel));
			Audio::ErrorCheck(channel->setVolume(volume));
			Audio::ErrorCheck(channel->setPitch(pitch));
		}
		Audio::ErrorCheck(channel->setPaused(false));
		audioCore->coreChannels[channelID] = channel;
	}
}

void SoundInstance::Stop()
{
	if (!this->isPlaying())
		return;

	auto foundChannel = audioCore->coreChannels.find(channelID);
	Audio::ErrorCheck((*foundChannel).second->stop());
}

bool SoundInstance::isPlaying()
{
	auto foundChannel = audioCore->coreChannels.find(channelID);
	if (foundChannel == audioCore->coreChannels.end())
		return false;
	bool playingState;
	Audio::ErrorCheck((*foundChannel).second->isPlaying(&playingState));
	return playingState;
}

void SoundInstance::SetPaused(bool paused)
{
	if (!isPlaying())
		return;

	auto foundChannel = audioCore->coreChannels.find(channelID);
	Audio::ErrorCheck(foundChannel->second->setPaused(paused));
}

bool SoundInstance::isPaused()
{
	if (!isPlaying())
		return false;

	bool pauseState;
	auto foundChannel = audioCore->coreChannels.find(channelID);
	Audio::ErrorCheck(foundChannel->second->getPaused(&pauseState));
	return pauseState;
}

void SoundInstance::Set3DAttributes(Maths::Vector3 pos, Maths::Vector3 vel)
{
	attributes3D.pos = Audio::ToFMODVECTOR(pos);
	attributes3D.vel = Audio::ToFMODVECTOR(vel);

	if (!isPlaying())
		return;
	auto foundChannel = audioCore->coreChannels.find(channelID);
	Audio::ErrorCheck(foundChannel->second->set3DAttributes(&attributes3D.pos, &attributes3D.vel));
}

void SoundInstance::SetMaxMinDistance(float max, float min)
{
	distances.max = max;
	if (min == 0)
		distances.min = min;
}

void SoundInstance::SetObjectMinDistance(const Maths::Vector3& scale)
{
	auto temMax = std::max(scale.x, scale.y);
	distances.min = std::max(temMax, scale.z)*1.5f;
}



