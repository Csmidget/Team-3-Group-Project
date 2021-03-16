#pragma once
#include"PlaySound.h"
#include"../Audio/SoundManager.h"
#include"../Engine/GameObject.h"

using namespace NCL;
using namespace CSC8508;

enum class PlaySound::PlayMode {
	OnStart,
	OnKill,
	OnCollisionBegin,
	OnCollisionEnd,
};

PlaySound::PlaySound(GameObject* object, std::string path, int mode, float volume, float minDistance)
	:Component("PlaySound", object)
{
	soundPath = path;
	soundVolume = volume;

	if (mode == 0) this->mode = PlayMode::OnStart;
	else if (mode == 1) this->mode = PlayMode::OnCollisionBegin;
	else if (mode == 2) this->mode = PlayMode::OnCollisionEnd;
	else if (mode == 3) this->mode = PlayMode::OnKill;

	sound = new Audio::SoundInstance();
	Audio::SoundManager::CreateInstance(soundPath, sound);
	sound->SetVolume(soundVolume);
	sound->SetMaxMinDistance(200, minDistance);
	sound->Set3DAttributes(object->GetTransform().GetPosition());
}

PlaySound::~PlaySound()
{
	delete sound;
}

void PlaySound::OnCollisionBegin(GameObject* otherObject)
{
	if (mode == PlayMode::OnCollisionBegin)
			sound->Play();
}

void PlaySound::OnCollisionEnd(GameObject* otherObject)
{
	if (mode == PlayMode::OnCollisionEnd)
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
