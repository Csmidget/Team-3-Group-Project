#include "MoveRightComponent.h"

#include "../Engine/GameObject.h"

using namespace NCL;
using namespace CSC8508;

MoveRightComponent::MoveRightComponent(GameObject* object) : Component(object) {
	physicsObject = object->GetPhysicsObject();
	dir = Vector3(10, 0, 0);
};


void MoveRightComponent::Update(float dt) {
	physicsObject->AddForce(dir);
}
