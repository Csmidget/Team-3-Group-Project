#pragma once
#include"Core.h"
#include"SoundManager.h"
#include"SoundInstance.h"
#include"../Engine/Transform.h"
#include"../../Common/Assets.h"

using namespace NCL;

namespace NCL { namespace CSC8508{ namespace Audio{ namespace SoundManager{ namespace {
	Core* audioCore = nullptr;
}}}}}

using namespace NCL::CSC8508::Audio;

void SoundManager::Init()
{
	if (audioCore != nullptr)
		return;
	audioCore = new Core();
	
}

void SoundManager::Update()
{
	audioCore->Update();
}

void SoundManager::Release()
{
	delete audioCore;
}

void SoundManager::PlayOneShot(const std::string& soundFile, const Maths::Vector3& position)
{
	auto channelID = audioCore->coreNextChannelID++;
	if (audioCore == nullptr)
		return;
	FMOD::Sound* sound;
	FMOD::Channel* channel;
	ErrorCheck(audioCore->coreSystem->createSound((Assets::AUDIODIR + soundFile).c_str(), FMOD_3D | FMOD_LOOP_OFF | FMOD_3D_LINEARSQUAREROLLOFF, 0, &sound));
	Audio::ErrorCheck(audioCore->coreSystem->playSound(sound, 0, true, &channel));
	if (channel)
	{
		FMOD_VECTOR pos = Audio::ToFMODVECTOR(position);
		FMOD_VECTOR vel = Audio::ToFMODVECTOR(Maths::Vector3(0, 0, 0));
		Audio::ErrorCheck(channel->set3DAttributes(&pos, &vel));
		Audio::ErrorCheck(channel->setPaused(false));
	}
	channelID = audioCore->coreNextChannelID++;
	audioCore->coreChannels[channelID] = channel;
	audioCore->oneShots[channel] = sound;	
}

void SoundManager::CreateInstance(const std::string& soundFile, SoundInstance* soundInstnce)
{
	if (audioCore == nullptr)
		return;
	if (soundInstnce == nullptr)
		soundInstnce = new SoundInstance();

	auto foundSound = audioCore->coreSounds.find(soundFile);

	if (foundSound != audioCore->coreSounds.end())
	{
		soundInstnce->sound = (*foundSound).second.at(0)->sound;
		soundInstnce->audioCore = (*foundSound).second.at(0)->audioCore;
		soundInstnce->path = (*foundSound).second.at(0)->path;
		(*foundSound).second.emplace_back(soundInstnce);
		return;
	}
	
	ErrorCheck(audioCore->coreSystem->createSound((Assets::AUDIODIR + soundFile).c_str(), FMOD_3D | FMOD_LOOP_OFF| FMOD_3D_LINEARSQUAREROLLOFF, 0, &soundInstnce->sound));
	soundInstnce->audioCore = audioCore;
	soundInstnce->path = soundFile;
	audioCore->coreSounds[soundFile].emplace_back(soundInstnce);
}

void SoundManager::DeleteInstance(SoundInstance* soundInstance)
{
	auto foundSound = audioCore->coreSounds.find(soundInstance->path);
	auto& foundSoundVector = (*foundSound).second;
	auto foundSoundInstance = std::find(foundSoundVector.begin(), foundSoundVector.end(), soundInstance);

	if (foundSoundVector.size() == 1)
		(*foundSoundInstance)->sound->release();

	foundSoundVector.erase(foundSoundInstance);
}

void SoundManager::UpdateListener(const SoundListener* listener)
{
	ErrorCheck(audioCore->coreSystem->set3DListenerAttributes(listener->ID, &listener->pos, &listener->vel, &listener->forward, &listener->up));
}

void SoundManager::AddListener()
{
	audioCore->listenersNumber++;
	ErrorCheck(audioCore->coreSystem->set3DNumListeners(audioCore->listenersNumber));
}

void SoundManager::RemoveListener()
{
	if (audioCore->listenersNumber == 1)
		return;
	audioCore->listenersNumber--;
	ErrorCheck(audioCore->coreSystem->set3DNumListeners(audioCore->listenersNumber));
}

void SoundManager::StopAllInstances()
{
	for (auto channel : audioCore->coreChannels)
		ErrorCheck(channel.second->stop());
}

bool SoundManager::IsInit()
{
	return audioCore == nullptr ? false : true;
}

void SoundManager::Set3DSetting(float distanceFactor, float rollofscale, float dopplerScale)
{
	Audio::ErrorCheck(audioCore->coreSystem->set3DSettings(dopplerScale, distanceFactor, rollofscale));
}

SoundListener::SoundListener(int id):
	ID(id)
{ 
	pos = ToFMODVECTOR(Maths::Vector3(0, 0, 0));
	vel = ToFMODVECTOR(Maths::Vector3(0, 0, 0));
	up = ToFMODVECTOR(Maths::Vector3(0, 1, 0));
	forward = ToFMODVECTOR(Maths::Vector3(0, 0, -1));
}

void SoundListener::UpdateAttributes(const Maths::Vector3& position, const Maths::Vector3& velocity, const Maths::Vector3& _forward, const Maths::Vector3& _up)
{
	pos = Audio::ToFMODVECTOR(position);
	forward = Audio::ToFMODVECTOR(_forward.Normalised());
	up = Audio::ToFMODVECTOR(_up.Normalised());
	vel = Audio::ToFMODVECTOR(velocity);
}
