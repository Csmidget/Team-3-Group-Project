#pragma once
#include"../../../Common/Vector3.h"
#include"Core.h"
#include"SoundInstance.h"


namespace NCL {
	namespace CSC8508 {
		namespace Audio {

			class SoundInstance;

			namespace SoundManager {

				void Init();
				void Update();
				void Release();
				int  ErrorCheck(FMOD_RESULT result);
				bool IsInit();

				void PlayOneShot(const std::string& soundFile, const Maths::Vector3& position);
				void CreateInstance(const std::string& soundFile, SoundInstance* soundInstnce);
				void DeleteInstance(SoundInstance* soundInstance);

				void SetListenerParameters(const Maths::Vector3& pos, const Maths::Vector3& vel = Maths::Vector3(0,0,0));
				void StopAllInstances();
			}			
		}
	}
}