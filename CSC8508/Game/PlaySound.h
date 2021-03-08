#pragma once
#include "../Engine/Component.h"
#include<iostream>
#include"../Audio/SoundInstance.h"

namespace NCL {
	namespace CSC8508 {



		class PlaySound : public Component {
		public:

			enum class PlayMode;

			PlaySound(GameObject* object, std::string path, std::string mode, float volume);
			~PlaySound();
			void OnCollisionBegin(GameObject* otherObject);
			void OnCollisionEnd(GameObject* otherObject);
			void OnKill();
			void Start();

		private:
			Audio::SoundInstance* sound;

			int soundVolume;
			std::string soundPath;
			PlayMode mode;
		};

	}
}