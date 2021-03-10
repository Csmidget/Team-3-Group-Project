#include "TeleportComponent.h"
#include "../Engine/GameObject.h"
#include "../Engine/Transform.h"
#include "../Engine/PhysicsObject.h"

using namespace NCL;
using namespace CSC8508;

TeleportComponent::TeleportComponent(GameObject* object) : Component(object) {
	otherTeleportPosition = {};
}

void TeleportComponent::Start() {
	otherTeleportPosition = transform->GetPosition();
}


/*void TeleportComponent::OnTeleport() {

	transform->SetPosition(otherTeleportPosition);

	PhysicsObject* physicsObject = gameObject->GetPhysicsObject();
	if (physicsObject != nullptr) {
		physicsObject->SetLinearVelocity(Vector3());
		physicsObject->SetAngularVelocity(Vector3());
	}

	transform->SetOrientation({});

	gameObject->SetIsActive(true);
}*/

void TeleportComponent::OnCollisionBegin(GameObject* other)
{
	if (other->HasTag("Player"))
	{
		transform->SetPosition(otherTeleportPosition);

		PhysicsObject* physicsObject = gameObject->GetPhysicsObject();
		if (physicsObject != nullptr) {
			physicsObject->SetLinearVelocity(Vector3());
			physicsObject->SetAngularVelocity(Vector3());
		}

		transform->SetOrientation({});

		gameObject->SetIsActive(true);
	}
}