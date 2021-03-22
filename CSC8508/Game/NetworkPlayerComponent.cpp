#include "NetworkPlayerComponent.h"
#include "../Engine/Transform.h"
#include "../Engine/GameObject.h"
#include "../Engine/PhysicsObject.h"
using namespace NCL;
using namespace CSC8508;
using namespace Maths;

NCL::CSC8508::NetworkPlayerComponent::NetworkPlayerComponent(GameObject* object) : Component("NetworkPlayerComponent", object)
{

}

void NCL::CSC8508::NetworkPlayerComponent::Update(float dt)
{
	gameObject->GetPhysicsObject()->SetAngularVelocity(Vector3(0, 0, 0));
	gameObject->GetTransform().SetOrientation(orientation);
}


