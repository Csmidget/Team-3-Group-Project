#pragma once
#include "../Engine/Component.h"

#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class PhysicsObject;

		class MoveComponent : public Component {

		public:
			MoveComponent(GameObject* object, Maths::Vector3 direction, float time);
			void Update(float dt);

		private:
			bool flip;
			float timePerDirection;
			float currentTimer;

			Maths::Vector3 dir;
			PhysicsObject* physicsObject;
		};

	}
}