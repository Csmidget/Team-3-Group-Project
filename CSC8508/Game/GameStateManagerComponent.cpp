#include "GameStateManagerComponent.h"
#include "Game.h"
#include "../Engine/GameWorld.h"
#include "GameTechRenderer.h"

GameStateManagerComponent* GameStateManagerComponent::instance = nullptr;

GameStateManagerComponent::GameStateManagerComponent(GameObject* object, Game* game) : Component("GameStateManagerComponent", object)
{
	if (instance != nullptr)
		gameObject->Destroy();
	else
		instance = this;

	this->game = game;
	isGameFinished = false;
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

	if (!majorityFinished && game->IsMajorityPlayersFinished())
	{
		majorityFinished = true;
		finishTimer = 15.0f;
	}

	if (majorityFinished)
	{
		game->getRenderer()->DrawString("Time remaining: " + std::to_string((int)finishTimer),Vector2(85,90),Vector4(1.0f,1.0f,1.0f,1.0f));
		finishTimer -= dt;
		if (finishTimer <= 0.0f)
			isGameFinished = true;
	}

	if (game->IsNetworkGame())
		isGameFinished = isGameFinished || game->IsAllPlayersFinished();
	else
		isGameFinished = isGameFinished || isPlayerFinished;

	int score = ScoreComponent::instance ? ScoreComponent::instance->GetScore() : 0;
	if(!isGameFinished) Debug::Print("Score: " + std::to_string(score), Vector2(85, 95));


	//if (IsGameOver() && isGameFinished) isGameFinished = true;
	//Go to next scene
}

bool GameStateManagerComponent::IsGameOver()
{
//	if (players.size() == 0) return false;
//
//	for (int i = 0; i < players.size(); i++)
//		if (!players.at(i)->GetComponent<ScoreComponent>()->IsFinished())
//			return false;

//	return true;
	return false;
}
