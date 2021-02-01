#pragma once
#include "Constraint.h"
#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {
		class GameObject;

		using namespace Maths;

		class AngularImpulseConstraint : public Constraint {
		public:
			AngularImpulseConstraint(GameObject* obj, Vector3 force) {
				object = obj;
				this->force = force;
			}
			~AngularImpulseConstraint() {};

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* object;
			Vector3 force;

		};
	}
}