#pragma once
#include"External/inc/fmod.hpp"
#include<map>
#include<vector>
#include<iostream>

namespace NCL {
	namespace CSC8508 {
		namespace Audio {

			namespace SoundManager {
				int ErrorCheck(FMOD_RESULT result);
			}

			class SoundInstance;
	
			struct Core {

				FMOD::System* coreSystem;
				int coreNextChannelID;

				typedef std::map<std::string, std::vector<SoundInstance*>> SoundMap;
				typedef std::map<int, FMOD::Channel*> ChannelMap;

				SoundMap coreSounds;
				ChannelMap coreChannels;

				Core() :
					coreSystem(nullptr), coreNextChannelID(0)
				{
					SoundManager::ErrorCheck(FMOD::System_Create(&coreSystem));
					SoundManager::ErrorCheck(coreSystem->init(512, FMOD_INIT_NORMAL, NULL));
				}
				~Core()
				{
					SoundManager::ErrorCheck(coreSystem->release());
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

					SoundManager::ErrorCheck(coreSystem->update());
				}
			};
		}
	}
}
