#include "PlayerRayFeetComponent.h"
#include "../Game/Game.h"
#include "../Engine/GameObject.h"
#include "DisappearingPlatformComponent.h"
#include "../Engine/Debug.h"
using namespace NCL;
using namespace CSC8508;
using namespace physics;

NCL::CSC8508::PlayerRayFeetComponent::PlayerRayFeetComponent(GameObject* object, Game* game) : Component("PlayerRayFeetComponent", object)
{
	this->game = game;
}

void NCL::CSC8508::PlayerRayFeetComponent::Update(float dt)
{
	GameObject* hit = game->Raycast(gameObject->GetTransform().GetPosition(), gameObject->GetTransform().GetPosition() + Vector3(0, -1, 0) * 1.2f);

	if (hit && hit->GetComponent<DisappearingPlatformComponent>())
	{
		hit->GetComponent<DisappearingPlatformComponent>()->collided = true;
	}
}
