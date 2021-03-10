#include "RespawningObject.h"

using namespace NCL;
using namespace CSC8508;

RespawningObject::RespawningObject(Vector3 spawnPosition, bool randomizeOrientation, string name) : GameObject(name) {
	this->spawnPosition = spawnPosition;
	this->randomizeOrientation = randomizeOrientation;
}

void RespawningObject::OnKill() {
	transform.SetPosition(spawnPosition);
	
	if (physicsObject) {
		physicsObject->SetLinearVelocity(Vector3());
		physicsObject->SetAngularVelocity(Vector3());
	}

	if (!randomizeOrientation) {
		transform.SetOrientation({});
	}

	else {
		Quaternion newRot = 
			Matrix3::Rotation((float)(rand() % 360), Vector3(1, 0, 0)) *
			Matrix3::Rotation((float)(rand() % 360), Vector3(0, 1, 0)) *
			Matrix3::Rotation((float)(rand() % 360), Vector3(0, 0, 1));

		transform.SetOrientation(newRot);
	}
}
