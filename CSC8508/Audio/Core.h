#pragma once
#include"External/inc/fmod.hpp"
#include<map>
#include<vector>
#include<iostream>

namespace NCL {
	namespace CSC8508 {
		namespace Audio {

			class SoundInstance;

			int ErrorCheck(FMOD_RESULT result);
			
			
			struct Core {

				FMOD::System* coreSystem;
				int coreNextChannelID;

				typedef std::map<std::string, std::vector<SoundInstance*>> SoundMap;
				typedef std::map<unsigned int, FMOD::Channel*> ChannelMap;

				SoundMap coreSounds;
				ChannelMap coreChannels;

				Core();
				~Core();

				void Update();
			};
		}
	}
}
