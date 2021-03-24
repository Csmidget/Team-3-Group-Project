#pragma once
#include "../Engine/Component.h"
#include <string>

namespace NCL {
	namespace CSC8508 {

		namespace Audio {
			class SoundInstance;
		}

		class PlaySound : public Component {
		public:

			enum class PlayMode;

			PlaySound(GameObject* object, std::string path, int mode, float volume, float minDistance);
			~PlaySound();
			void OnCollisionBegin(GameObject* otherObject);
			void OnCollisionEnd(GameObject* otherObject);
			void OnKill();
			void Start();

		private:
			Audio::SoundInstance* sound;

			float soundVolume;
			std::string soundPath;
			PlayMode mode;
		};

	}
}