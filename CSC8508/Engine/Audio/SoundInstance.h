#pragma once
#include"Core.h"
#include"../../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {
		namespace Audio {

			namespace SoundManager {
				void CreateInstance(const std::string& path, SoundInstance* soundInstnce);
				void DeleteInstance(SoundInstance* soundInstance);
			}
			struct Audio_3DAttributes {

				FMOD_VECTOR pos;
				FMOD_VECTOR vel;
			};
			
	
			
			class SoundInstance {
			public:

				SoundInstance();
				~SoundInstance(); //release

				int Play();
				void Stop();
				int isPlaying();
				
				void SetPaused();
				bool isPaused();

				void SetVolume();
				float GetVolume();
				void SetPitch();
				float GetPith();

				void Set3DAtribbutes();
				void SetMode();
				void SetAudiabledistance(float max, float min);

				static FMOD_VECTOR ToFMODVECTOR(const Maths::Vector3& v);
				static float FloatTodB(float volume);
				static float dBtoFloat(float dB);

			protected:

				FMOD::Sound* sound;
				Core* audioCore;
				std::string path;
				FMOD_VECTOR position;
				Audio_3DAttributes attributes3D;
				float volume;
				float pitch;
				int channelID;

				friend void SoundManager::CreateInstance(const std::string& path, SoundInstance* soundInstnce);
				friend void SoundManager::DeleteInstance(SoundInstance* soundInstance);
			};
		}
	}
}
