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
				int ErrorCheck(FMOD_RESULT result);
				bool IsInit();
				bool HasListener();

				void PlayOneShot(const std::string& soundFile, const Maths::Vector3& position);
				void CreateInstance(const std::string& soundFile, SoundInstance* soundInstnce);
				void SetListener();

				void PauseAllInstances();
			}			
		}
	}
}