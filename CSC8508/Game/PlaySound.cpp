#pragma once
#include"PlaySound.h"
#include"../Audio/SoundManager.h"

using namespace NCL;
using namespace CSC8508;

enum class PlaySound::PlayMode {
	OnStart,
	OnKill,
	OnCollisionBegin,
	OnCollisionEnd,
};

PlaySound::PlaySound(GameObject* object, std::string path, std::string mode, float volume)
	:Component(object)
{
	soundPath = path;
	soundVolume = volume;

	if (mode == "OnStart") this->mode = PlayMode::OnStart;
	else if (mode == "OnCollisionBegin") this->mode = PlayMode::OnCollisionBegin;
	else if (mode == "OnCollisionEnd") this->mode = PlayMode::OnCollisionEnd;
	else if (mode == "OnKill") this->mode = PlayMode::OnKill;

	sound = new Audio::SoundInstance();
	Audio::SoundManager::CreateInstance(soundPath, sound);
	sound->SetVolume(soundVolume);
}

PlaySound::~PlaySound()
{
	delete sound;
}

void PlaySound::OnCollisionBegin(GameObject* otherObject)
{
	if (mode == PlayMode::OnCollisionBegin)
		if (!sound->isPlaying())
			sound->Play();
}

void PlaySound::OnCollisionEnd(GameObject* otherObject)
{
	if (mode == PlayMode::OnCollisionEnd)
		if (!sound->isPlaying())
			sound->Play();
}

void PlaySound::OnKill()
{
	if (mode == PlayMode::OnKill)
			sound->Play();
}

void NCL::CSC8508::PlaySound::Start()
{
	if (mode == PlayMode::OnStart)
			sound->Play();
}
