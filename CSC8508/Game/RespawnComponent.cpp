#include "RespawnComponent.h"
#include "../Engine/Transform.h"
#include "../Engine/GameObject.h"
#include "../Engine/PhysicsObject.h"

using namespace NCL;
using namespace CSC8508;
using namespace Maths;

RespawnComponent::RespawnComponent(GameObject* object) : Component(object) {
	spawnPosition = {};
}

void RespawnComponent::Start() {
	spawnPosition = transform->GetPosition();
}

void RespawnComponent::OnKill() {

	transform->SetPosition(spawnPosition);

	PhysicsObject* physicsObject = gameObject->GetPhysicsObject();
	if (physicsObject != nullptr) {
		physicsObject->SetLinearVelocity(Vector3());
		physicsObject->SetAngularVelocity(Vector3());
	}

	transform->SetOrientation({});

	gameObject->SetIsActive(true);
}