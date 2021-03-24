#include "NetworkPlayerComponent.h"
#include "../Engine/Transform.h"
#include "../Engine/GameObject.h"
#include "../Engine/PhysicsObject.h"
#include "GameStateManagerComponent.h"
using namespace NCL;
using namespace CSC8508;
using namespace Maths;

NCL::CSC8508::NetworkPlayerComponent::NetworkPlayerComponent(GameObject* object, int id) : Component("NetworkPlayerComponent", object)
{
	playerID = id;
	object->SetPersistence(true);
	timeFrame = 0.1f;
}

void NCL::CSC8508::NetworkPlayerComponent::Update(float dt)
{
	progress = std::min(1.0f, progress + (dt / timeFrame));

	gameObject->GetTransform().SetOrientation(orientation);
	transform->SetPosition(Vector3::Lerp(progress, previousPosition, targetPosition));

	gameObject->GetPhysicsObject()->SetAngularVelocity(Vector3(0, 0, 0));
	gameObject->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 0));

	if (GameStateManagerComponent::instance->IsGameFinished() && gameObject->IsActive())
		gameObject->SetIsActive(false);

}

void NCL::CSC8508::NetworkPlayerComponent::SetTargetPosition(Vector3 target) {
	targetPosition = target;
	previousPosition = transform->GetPosition();
	progress = 0;
}

void NCL::CSC8508::NetworkPlayerComponent::SetIsFinished(bool isFinished) {

	if (!GameStateManagerComponent::instance->IsGameFinished()) {
		if (isFinished) {
			if (!isLevelFinished) std::cout << "Player " << std::to_string(playerID) << " has finished with score " << score << std::endl;
		}
		gameObject->SetIsActive(!isFinished);
		isLevelFinished = isFinished;
	}
}

void NCL::CSC8508::NetworkPlayerComponent::OnActive()
{
	isLevelFinished = false;
	gameObject->GetPhysicsObject()->body->makeKinematic();
}
