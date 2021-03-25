#include "RingComponent.h"
#include "../Engine/GameObject.h"

using namespace NCL;
using namespace CSC8508;

RingComponent::RingComponent(GameObject* object, int bonus) : Component("RingComponent", object)
{
	//active = true;
	physicsObject = object->GetPhysicsObject();
	physicsObject->body->makeTrigger();
	this->bonus = bonus;
}

std::vector<std::string> RingComponent::DebugInfo() {
	std::vector<std::string> returnVec;

	returnVec.push_back("  Bonus:" + std::to_string(bonus));

	return returnVec;
}
