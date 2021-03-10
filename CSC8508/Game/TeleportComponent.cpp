#include "TeleportComponent.h"
#include "../Engine/Transform.h"
#include "../Engine/GameObject.h"
#include "../Engine/PhysicsObject.h"

using namespace NCL;
using namespace CSC8508;
using namespace Maths;

TeleportComponent::TeleportComponent(GameObject* object) : Component(object) {
	otherTeleportPosition = {};
}

void TeleportComponent::Start() {
	otherTeleportPosition = Vector3(10, -10, 10);
//	otherTeleportPosition = transform->GetPosition();
}

void TeleportComponent::OnCollisionBegin(GameObject* other)
{
	if (other->HasTag("Player"))
	{
		std::cout << "teleport\n";
		std::cout << otherTeleportPosition;
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