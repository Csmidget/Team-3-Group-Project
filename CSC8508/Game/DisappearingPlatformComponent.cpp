#include "DisappearingPlatformComponent.h"

NCL::CSC8508::DisappearingPlatformComponent::DisappearingPlatformComponent(GameObject* object) : Component("DisappearingPlatform", object)
{
	timer = MAX_TIMER;
	collided = false;
}

void NCL::CSC8508::DisappearingPlatformComponent::Update(float dt)
{
	if (collided)
	{
		timer -= dt;
		//start disappearing
	}
}

void NCL::CSC8508::DisappearingPlatformComponent::OnCollisionBegin(GameObject* otherObject)
{
	if (!collided)
	{
		collided = true;
	}
}

void NCL::CSC8508::DisappearingPlatformComponent::Disappear()
{

}
