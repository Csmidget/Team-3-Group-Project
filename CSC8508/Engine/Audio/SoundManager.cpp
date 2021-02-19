#pragma once
#include"SoundManager.h"
#include"SoundInstance.h"

//FMOD System wrapped with Struct
namespace NCL { namespace CSC8508{ namespace Audio{ namespace SoundManager{
namespace {

	struct Core {

		FMOD::System* coreSystem;
		int coreNextChannelID;

		typedef std::map<std::string, std::vector<SoundInstance*>> SoundMap;
		typedef std::map<int, FMOD::Channel*> ChannelMap;

		SoundMap coreSounds;
		ChannelMap coreChannels;

		Core(): 
			coreSystem(nullptr), coreNextChannelID(0)
		{	
			ErrorCheck(FMOD::System_Create(&coreSystem));
			ErrorCheck(coreSystem->init(512, FMOD_INIT_NORMAL, NULL));
		}
		~Core()
		{
			ErrorCheck(coreSystem->release());
		}

		void Update()
		{
			std::vector<ChannelMap::iterator> stoppedChannels;
			for (auto it = coreChannels.begin(); it != coreChannels.end(); ++it)
			{
				bool isChannelPlaying = false;
				it->second->isPlaying(&isChannelPlaying);
				if (!isChannelPlaying)
					stoppedChannels.push_back(it);
			}

			for (auto& it : stoppedChannels)
				coreChannels.erase(it);

			//Clears coreSound if no instances -  optional
			for (auto it = coreSounds.begin(); it != coreSounds.end(); ++it)
				if ((*it).second.size() == 0)
					coreSounds.erase(it);

			ErrorCheck(coreSystem->update());
		}
	};

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
		soundInstnce->system = (*foundSound).second.at(0)->system;
		(*foundSound).second.emplace_back(soundInstnce);
		return;
	}
	
	ErrorCheck(audioCore->coreSystem->createSound(soundFile.c_str(), FMOD_3D | FMOD_LOOP_OFF, 0, &soundInstnce->sound));
	soundInstnce->system = audioCore->coreSystem;
	audioCore->coreSounds[soundFile].emplace_back(soundInstnce);
}


void SoundManager::AddChannel(FMOD::Channel* channel)
{
	audioCore->coreChannels[audioCore->coreNextChannelID++] = channel;
}

int SoundManager::ErrorCheck(FMOD_RESULT result)
{
	if (result == FMOD_OK)
		return 0;

	std::cout << "FMOD ERROR: " << result << std::endl;
	return 1;
}