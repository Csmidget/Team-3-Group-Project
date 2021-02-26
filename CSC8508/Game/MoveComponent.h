#pragma once
#include "../Engine/Component.h"

#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class PhysicsObject;

		class MoveComponent : public Component {

		public:
			MoveComponent(GameObject* object, Maths::Vector3 direction);
			void Update(float dt);

		private:
			Maths::Vector3 dir;
			PhysicsObject* physicsObject;
		};

	}
}