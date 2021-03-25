#pragma once
#include "../Engine/Component.h"

#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class PhysicsObject;

		class RingComponent : public Component {

		public:
			RingComponent(GameObject* object , int bonus);
			int GetBonus() { return bonus; }
		protected:
			std::vector<std::string> DebugInfo() override;

		private:
			PhysicsObject* physicsObject;
			int bonus;
		};

	}
}