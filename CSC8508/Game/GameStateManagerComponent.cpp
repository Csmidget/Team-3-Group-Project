#define NOMINMAX
#include "GameStateManagerComponent.h"
#include "Game.h"
#include "ScoreComponent.h"
#include "GameTechRenderer.h"
#include "../Engine/GameObject.h"

using namespace NCL;
using namespace CSC8508;


GameStateManagerComponent* GameStateManagerComponent::instance = nullptr;

GameStateManagerComponent::GameStateManagerComponent(GameObject* object, Game* game) : Component("GameStateManagerComponent", object)
{
	if (instance != nullptr)
		gameObject->Destroy();
	else
		instance = this;

	this->game = game;
	isGameFinished = false;
	levelID = 0;
}

GameStateManagerComponent::~GameStateManagerComponent()
{
	if (instance == this)
		instance = nullptr;
}

void GameStateManagerComponent::Start()
{

}

void NCL::CSC8508::GameStateManagerComponent::Update(float dt) {

	if (levelID != 3) {
		if (!majorityFinished && game->IsMajorityPlayersFinished())
		{
			finishTimer = 15.0f;
		}

		majorityFinished = game->IsMajorityPlayersFinished();

		if (majorityFinished)
		{
			finishTimer = std::max(0.0f, finishTimer - dt);
			if (!isGameFinished) game->getRenderer()->DrawString("Time remaining: " + std::to_string((int)finishTimer), Vector2(80, 90), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 12.0f);
			if (finishTimer <= 0.0f)
				isGameFinished = true;
		}
	}

	if (game->IsNetworkGame())
		isGameFinished = isGameFinished || game->IsAllPlayersFinished();
	else
		isGameFinished = isGameFinished || isPlayerFinished;

	int score = ScoreComponent::instance ? ScoreComponent::instance->GetScore() : 0;
	if(!isGameFinished) game->getRenderer()->DrawString("Score: " + std::to_string(score), Vector2(85, 95), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 12.0f);
}
