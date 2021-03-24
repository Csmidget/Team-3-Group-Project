#include "LocalNetworkPlayerComponent.h"
#include "GameStateManagerComponent.h"
#include "ScoreComponent.h"
#include "../Engine/NetworkManager.h"

using namespace NCL;
using namespace CSC8508;

LocalNetworkPlayerComponent::LocalNetworkPlayerComponent(GameObject* object, LocalPlayer* localPlayer) : Component("LocalNetworkPlayerComponent",object)
{
	this->localPlayer = localPlayer;
}

void LocalNetworkPlayerComponent::SetTransform(const Vector3& pos, const Quaternion& orientation)
{
	transform->SetPosition(pos);
	transform->SetOrientation(orientation);
}

void LocalNetworkPlayerComponent::SetGameFinished(bool val)
{
	localPlayer->isFinished = val;
}

void LocalNetworkPlayerComponent::Update(float dt)
{
//	if (!localPlayer || !score) return;

	localPlayer->score = ScoreComponent::instance ? ScoreComponent::instance->GetScore() : 0;

	localPlayer->isFinished = GameStateManagerComponent::instance ? GameStateManagerComponent::instance->IsPlayerFinished() : false;
	//localPlayer->isFinished = score->IsFinished();
}

int NCL::CSC8508::LocalNetworkPlayerComponent::GetLocalPlayerID() const
{
	return localPlayer->player->GetPlayerID();
}
