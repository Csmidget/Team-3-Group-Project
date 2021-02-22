#pragma once
#include"SoundInstance.h"

using namespace NCL::CSC8508::Audio;

SoundInstance::SoundInstance():
	sound(nullptr), audioCore(nullptr), channelID(-1), volume(1), pitch(1), path("")
{
	attributes3D.pos = ToFMODVECTOR(Maths::Vector3(0,0,0));
	attributes3D.vel = ToFMODVECTOR(Maths::Vector3(0,0,0));
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
		FMOD_MODE mode;
		sound->getMode(&mode);

		/*if (mode & FMOD_3D)
			SoundManager::ErrorCheck(channel->set3DAttributes(&attributes3D.pos, &attributes3D.vel));
		SoundManager::ErrorCheck(channel->setVolume(volume));
		SoundManager::ErrorCheck(channel->setPaused(false));
		SoundManager::ErrorCheck(channel->setPitch(pitch));*/
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

FMOD_VECTOR SoundInstance::ToFMODVECTOR(const Maths::Vector3& v)
{
	FMOD_VECTOR vec;
	vec.x = v.x;
	vec.y = v.y;
	vec.z = v.z;
	return vec;
}
