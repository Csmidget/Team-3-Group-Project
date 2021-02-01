#include "LinearImpulseConstraint.h"
#include "GameObject.h"

using namespace NCL;
using namespace CSC8508;

void LinearImpulseConstraint::UpdateConstraint(float dt) {

	PhysicsObject* physA = object->GetPhysicsObject();

	if (physA)
		physA->ApplyLinearImpulse(force * dt);

}
