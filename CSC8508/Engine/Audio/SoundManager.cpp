#pragma once
#include"SoundManager.h"
#include"SoundInstance.h"


namespace NCL { namespace CSC8508{ namespace Audio{ namespace SoundManager{ namespace {
	Core* audioCore = nullptr;
}}}}}

using namespace NCL::CSC8508::Audio;

void SoundManager::Init()
{
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

void SoundManager::CreateInstance(const std::string& soundFile, SoundInstance* soundInstnce)
{
	if (soundInstnce == nullptr)
		soundInstnce = new SoundInstance();
	auto foundSound = audioCore->coreSounds.find(soundFile);

	if (foundSound != audioCore->coreSounds.end())
	{
		soundInstnce->sound = (*foundSound).second.at(0)->sound;
		soundInstnce->audioCore = (*foundSound).second.at(0)->audioCore;
		(*foundSound).second.emplace_back(soundInstnce);
		return;
	}
	
	ErrorCheck(audioCore->coreSystem->createSound(soundFile.c_str(), FMOD_3D | FMOD_LOOP_OFF, 0, &soundInstnce->sound));
	soundInstnce->audioCore = audioCore;
	audioCore->coreSounds[soundFile].emplace_back(soundInstnce);
}

int SoundManager::ErrorCheck(FMOD_RESULT result)
{
	if (result == FMOD_OK)
		return 0;

	std::cout << "FMOD ERROR: " << result << std::endl;
	return 1;
}