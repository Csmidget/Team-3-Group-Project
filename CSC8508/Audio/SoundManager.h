#pragma once
#include"../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class Transform;	

		namespace Audio {

			class SoundInstance;
			struct SoundListener;

			namespace SoundManager {

				void Init();
				void Update();
				void Release();
				bool IsInit();
				void Set3DSetting(float distanceFactor, float rollofscale = 1.0f, float dopplerScale = 1.0f);

				void PlayOneShot(const std::string& soundFile, const Maths::Vector3& position = Maths::Vector3(0,0,0)); //To implement
				void CreateInstance(const std::string& soundFile, SoundInstance* soundInstnce);
				void DeleteInstance(SoundInstance* soundInstance);

				void UpdateListener(const SoundListener* listener);
				void AddListener();
				void RemoveListener();
				void StopAllInstances();
			}	

		}
	}
}