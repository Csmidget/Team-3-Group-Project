#include "MoveComponent.h"

#include "../Engine/GameObject.h"

using namespace NCL;
using namespace CSC8508;

MoveComponent::MoveComponent(GameObject* object, Vector3 direction) : Component(object) {
	physicsObject = object->GetPhysicsObject();
	dir = direction;
};


void MoveComponent::Update(float dt) {
	physicsObject->AddForce(dir);
}
