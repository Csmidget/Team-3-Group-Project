#include "LocalNetworkPlayerComponent.h"

NCL::CSC8508::LocalNetworkPlayerComponent::LocalNetworkPlayerComponent(GameObject* object, LocalPlayer* localPlayer) : Component("LocalNetworkPlayerComponent",object)
{
	this->localPlayer = localPlayer;
	score = object->GetComponent<ScoreComponent>();
}

void NCL::CSC8508::LocalNetworkPlayerComponent::Update(float dt)
{
	if (!localPlayer || !score) return;

	localPlayer->score = score->GetScore();
	localPlayer->isFinished = score->IsFinished();
}
