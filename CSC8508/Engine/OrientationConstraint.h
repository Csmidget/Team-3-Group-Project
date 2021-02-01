#pragma once
#pragma once
#include "Constraint.h"
#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {
		class GameObject;

		using namespace Maths;

		class OrientationConstraint : public Constraint {
		public:
			OrientationConstraint(GameObject* o, GameObject* t, Vector3 axis) {
				object = o;
				targetObject = t;
				this->axis = axis;
			}
			~OrientationConstraint() {};

			void UpdateConstraint(float dt) override;

		protected:
			GameObject* object;
			GameObject* targetObject;

			Vector3 axis;
		};
	}
}