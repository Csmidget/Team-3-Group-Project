#include "GameStateManagerComponent.h"

NCL::CSC8508::GameStateManagerComponent::GameStateManagerComponent(GameObject* object, GameWorld* gameWorld) : Component("GameStateManagerComponent", object)
{
	world = gameWorld;

}

void NCL::CSC8508::GameStateManagerComponent::Start()
{
	RefreshPlayerList();

}

void NCL::CSC8508::GameStateManagerComponent::Update(float dt)
{
	if (clientPlayerID < players.size() && clientPlayerID >= 0) {
		int score = players.at(clientPlayerID)->GetComponent<ScoreComponent>()->GetScore();
		Debug::Print("Score: " + std::to_string(score), Vector2(75, 95));
		if (clientScore) *clientScore = score;
	
	}

	if (IsGameOver() && isGameFinished) *isGameFinished = true;
	//Go to next scene
}

void NCL::CSC8508::GameStateManagerComponent::RefreshPlayerList()
{
	players.clear();
	players = world->GetObjectsWithTag("Player");

	for (int i = 0; i < players.size(); i++)
		players.at(i)->AddComponent<ScoreComponent>();
}

bool NCL::CSC8508::GameStateManagerComponent::IsGameOver()
{
	for (int i = 0; i < players.size(); i++)
		if (!players.at(i)->GetComponent<ScoreComponent>()->IsFinished())
			return false;

	return true;
}
