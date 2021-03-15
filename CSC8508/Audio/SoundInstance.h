#pragma once
#include"Core.h"

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
			
			struct Audio_MaxMinDistance {
				float max;
				float min;
			};

			enum Audio_Mode {
				MODE_3D = 1,
				MODE_2D = 2,
				MODE_LOOP_ON = 4,
				MODE_LOOP_OFF = 8
			};
			
			class SoundInstance {
			public:

				SoundInstance();
				~SoundInstance(); //release

				void Play();
				void Stop();
				bool isPlaying();
				
				void SetPaused(bool paused);
				bool isPaused();

				inline void SetVolume(float v) { volume = v; } //Volume range[0-1] 
				inline float GetVolume() const { return volume; }
				inline void SetPitch(float p) { pitch = p; } //Pitch range [0.5-2]
				inline float GetPith() const { return pitch; }

				void Set3DAttributes(Maths::Vector3 pos, Maths::Vector3 vel = Maths::Vector3(0,0,0));
				void SetMaxMinDistance(float max, float min =0);
				void SetObjectMinDistance(const Maths::Vector3& scale);
				void SetLoop(bool loop) { isLoop = loop; }
				void Set3D(bool _3D) { is3D =_3D; }

			protected:

				FMOD::Sound* sound;
				Core* audioCore;
				std::string path;
				//FMOD_VECTOR position;
				Audio_3DAttributes attributes3D;
				Audio_MaxMinDistance distances;
				bool isLoop;
				bool is3D;
				float volume;
				float pitch;
				int channelID;

				friend void SoundManager::CreateInstance(const std::string& path, SoundInstance* soundInstnce);
				friend void SoundManager::DeleteInstance(SoundInstance* soundInstance);
			};
		}
	}
}
