#pragma once
#include"External/inc/fmod.hpp"
#include"../../Common/Vector3.h"
#include<map>
#include<vector>
#include<iostream>

namespace NCL {
	namespace CSC8508 {
		namespace Audio {

			class SoundInstance;
			int ErrorCheck(FMOD_RESULT result);
			FMOD_VECTOR ToFMODVECTOR(const NCL::Maths::Vector3& v);

			struct Core {

				FMOD::System* coreSystem;
				int coreNextChannelID;

				typedef std::map<std::string, std::vector<SoundInstance*>> SoundMap;
				typedef std::map<unsigned int, FMOD::Channel*> ChannelMap;

				std::map<FMOD::Channel*, FMOD::Sound*> oneShots;
				SoundMap coreSounds;
				ChannelMap coreChannels;

				Core();
				~Core();

				void Update();
			};
		}
	}
}
