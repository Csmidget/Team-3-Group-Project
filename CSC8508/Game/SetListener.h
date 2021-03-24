#pragma once
#include "../Engine/Component.h"

namespace NCL {

	namespace CSC8508 {

		namespace Audio {
			struct SoundListener;
		}

		class PhysicsObject;
		class GameObject;
		class SetListener : public Component {

		public:
			SetListener(GameObject* object, int listenerID);
			~SetListener();
			void Update(float dt) override;

		private:
			Audio::SoundListener* listener;
			PhysicsObject* physicsObject;
			GameObject* object;
		};

	}
}
