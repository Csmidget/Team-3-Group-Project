#include "DisappearingPlatformComponent.h"
#include "../Engine/GameObject.h"
#include "../Engine/RenderObject.h"

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
		Disappear();
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
	gameObject->GetRenderObject()->SetColour(Vector4(1.f, 1.f, 1.f, 1.f) * (timer / MAX_TIMER));
	if (timer <= 0.f)
		gameObject->SetIsActive(false);
}
