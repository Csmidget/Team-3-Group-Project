#pragma once
#include "../Engine/Component.h"

#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class PhysicsObject;

		class MoveLeftComponent : public Component {

		public:
			MoveLeftComponent(GameObject* object);
			void Update(float dt);

		private:
			Maths::Vector3 dir;
			PhysicsObject* physicsObject;
		};

	}
}