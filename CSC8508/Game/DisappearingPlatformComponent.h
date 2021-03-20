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
		private:
			const float MAX_TIMER = 2.5f;
			float timer;
			bool collided;

			void Disappear();
		};
	}
}