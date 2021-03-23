#include "NetworkPlayerComponent.h"
#include "../Engine/Transform.h"
#include "../Engine/GameObject.h"
#include "../Engine/PhysicsObject.h"
using namespace NCL;
using namespace CSC8508;
using namespace Maths;

NCL::CSC8508::NetworkPlayerComponent::NetworkPlayerComponent(GameObject* object, int id) : Component("NetworkPlayerComponent", object)
{
	playerID = id;
	object->SetPersistence(true);
}

void NCL::CSC8508::NetworkPlayerComponent::Update(float dt)
{
	gameObject->GetPhysicsObject()->SetAngularVelocity(Vector3(0, 0, 0));
	gameObject->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 0));
	gameObject->GetTransform().SetOrientation(orientation);
}

void NCL::CSC8508::NetworkPlayerComponent::SetIsFinished(bool isFinished) {
	if (isFinished) {
		if(!isLevelFinished) std::cout << "Player " << std::to_string(playerID) << " has finished with score " << score << std::endl;
	}
	
	isLevelFinished = isFinished; 
}

void NCL::CSC8508::NetworkPlayerComponent::OnActive()
{
	isLevelFinished = false;
}
