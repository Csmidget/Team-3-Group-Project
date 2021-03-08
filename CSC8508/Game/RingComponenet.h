#pragma once
#include "../Engine/Component.h"

#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class PhysicsObject;

		class RingComponent : public Component {

		public:
			RingComponent(GameObject* object);
			void OnCollisionBegin(GameObject* otherObject) override;

		private:
			bool active = true;
			PhysicsObject* physicsObject;
		};

	}
}