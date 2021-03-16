#include "MoveComponent.h"

#include "../Engine/GameObject.h"

using namespace NCL;
using namespace CSC8508;

MoveComponent::MoveComponent(GameObject* object, Vector3 direction, float time) : Component("MoveComponent", object) {
	physicsObject = object->GetPhysicsObject();
	this->timePerDirection = time;
	this->currentTimer = 0.0f;
	this->flip = false;
	dir = direction;
};


void MoveComponent::Update(float dt) {
	
	currentTimer += dt;

	if (currentTimer > timePerDirection) {
		currentTimer -= timePerDirection;
		flip = !flip;
	}

	flip ?	physicsObject->AddForce(-dir) : physicsObject->AddForce(dir);
}
