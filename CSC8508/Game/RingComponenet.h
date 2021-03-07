#pragma once
#include "../Engine/Component.h"

#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class PhysicsObject;

		class RingComponent : public Component {

		public:
			RingComponent(GameObject* object, Maths::Vector3 direction, float time);
			void Update(float dt);
			void OnCollisionBegin(GameObject* otherObject);

		private:
			PhysicsObject* physicsObject;
		};

	}
}