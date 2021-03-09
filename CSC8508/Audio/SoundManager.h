#pragma once
#include"../../Common/Vector3.h"
#include"../Audio/External/inc/fmod_common.h"



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


			struct SoundListener {

				int ID;
				FMOD_VECTOR pos;
				FMOD_VECTOR vel;
				FMOD_VECTOR forward;
				FMOD_VECTOR up;


				SoundListener(int id);
				~SoundListener() {}
				void UpdateAttributes(const Maths::Vector3& position,
					const Maths::Vector3& velocity = Maths::Vector3(0, 0, 0),
					const Maths::Vector3& _forward = Maths::Vector3(0, 0, -1),
					const Maths::Vector3& _up = Maths::Vector3(0, 1, 0));
											
		
			};
		}
	}
}