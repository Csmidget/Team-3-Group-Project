#include "AngularImpulseConstraint.h"
#include "GameObject.h"

using namespace NCL;
using namespace CSC8508;

void AngularImpulseConstraint::UpdateConstraint(float dt) {

	PhysicsObject* physA = object->GetPhysicsObject();

	if (physA)
		physA->ApplyAngularImpulse(force * dt);

}
