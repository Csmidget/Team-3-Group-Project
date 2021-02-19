#pragma once
#include"SoundInstance.h"

using namespace NCL::CSC8508::Audio;

int SoundInstance::Play()
{
	//int ChannelID = SoundManager::GetFreeChannel();
	FMOD::Channel* channel = nullptr;

	SoundManager::ErrorCheck(system->playSound(sound, 0, true, &channel));
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
		SoundManager::AddChannel(channel);
	}
	return 0;
}
