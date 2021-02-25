#pragma once
#pragma once
#include "../Engine/Component.h"

#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class PhysicsObject;

		class MoveRightComponent : public Component {

		public:
			MoveRightComponent(GameObject* object);
			void Update(float dt);

		private:
			Maths::Vector3 dir;
			PhysicsObject* physicsObject;
		};

	}
}