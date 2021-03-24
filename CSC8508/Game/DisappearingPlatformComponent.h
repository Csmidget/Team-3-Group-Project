#pragma once
#include "../Engine/Component.h"

namespace NCL {
	namespace CSC8508 {

		class DisappearingPlatformComponent : public Component
		{
		public:
			DisappearingPlatformComponent(GameObject* object);

			void Update(float dt) override;
			void OnCollisionBegin(GameObject* otherObject) override;

			bool collided;

		private:
			const float MAX_TIMER = 1.2f;
			float timer;

			void Disappear();
		};
	}
}