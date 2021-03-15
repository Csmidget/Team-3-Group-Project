#include "TeleporterComponent.h"
#include "../Engine/GameObject.h"
using namespace NCL;
using namespace CSC8508;

TeleporterComponent::TeleporterComponent(GameObject* object, Maths::Vector3 target) : Component(object) {
	this->targetPosition = target;
}

void TeleporterComponent::OnCollisionStay(GameObject* other) {
	if (other->HasTag("Player"))
	{
		other->GetTransform().SetPosition(targetPosition);
	//	other->GetPhysicsObject()->body->setLinearVelocity(Vector3(0, 0, 0));
	}
}

