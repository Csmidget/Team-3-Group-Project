#pragma once
#include "Constraint.h"
#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {
		class GameObject;
		
		using namespace Maths;

		class LinearImpulseConstraint : public Constraint {
		public:
			LinearImpulseConstraint(GameObject* obj, Vector3 force) {
				object = obj;
				this->force = force;
			}
			~LinearImpulseConstraint() {};

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* object;
			Vector3 force;

		};
	}
}