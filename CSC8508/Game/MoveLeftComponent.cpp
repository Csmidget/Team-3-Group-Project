#include "MoveLeftComponent.h"

#include "../Engine/GameObject.h"

using namespace NCL;
using namespace CSC8508;

MoveLeftComponent::MoveLeftComponent(GameObject* object) : Component(object) {
	physicsObject = object->GetPhysicsObject();
	dir = Vector3(-10, 0, 0);
};


void MoveLeftComponent::Update(float dt){
	physicsObject->AddForce(dir);
}
