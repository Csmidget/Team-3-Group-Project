#include "GameStateManagerComponent.h"
#include "Game.h"
#include "../Engine/GameWorld.h"

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

	int score = ScoreComponent::instance ? ScoreComponent::instance->GetScore() : 0;
	if(!isGameFinished) Debug::Print("Score: " + std::to_string(score), Vector2(75, 95));


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
