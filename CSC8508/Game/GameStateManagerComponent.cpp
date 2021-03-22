#include "GameStateManagerComponent.h"
#include "Game.h"
#include "../Engine/GameWorld.h"

NCL::CSC8508::GameStateManagerComponent::GameStateManagerComponent(GameObject* object, Game* game) : Component("GameStateManagerComponent", object)
{
	this->game = game;
}

void NCL::CSC8508::GameStateManagerComponent::Start()
{

}

void NCL::CSC8508::GameStateManagerComponent::Update(float dt) {

	int score = ScoreComponent::instance ? ScoreComponent::instance->GetScore() : 0;
	Debug::Print("Score: " + std::to_string(score), Vector2(75, 95));


	if (IsGameOver() && isGameFinished) isGameFinished = true;
	//Go to next scene
}

bool NCL::CSC8508::GameStateManagerComponent::IsGameOver()
{
//	if (players.size() == 0) return false;
//
//	for (int i = 0; i < players.size(); i++)
//		if (!players.at(i)->GetComponent<ScoreComponent>()->IsFinished())
//			return false;

//	return true;
	return false;
}
