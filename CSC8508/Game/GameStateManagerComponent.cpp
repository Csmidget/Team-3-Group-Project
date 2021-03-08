#include "GameStateManagerComponent.h"

NCL::CSC8508::GameStateManagerComponent::GameStateManagerComponent(GameObject* object, GameWorld* gameWorld) : Component(object)
{
	world = gameWorld;

}

void NCL::CSC8508::GameStateManagerComponent::Start()
{
	RefreshPlayerList();

}

void NCL::CSC8508::GameStateManagerComponent::Update(float dt)
{
	if (clientPlayerID < players.size() && clientPlayerID >= 0)
		Debug::Print("Score: " + std::to_string(players.at(clientPlayerID)->GetComponent<ScoreComponent>()->GetScore()), Vector2(75, 95));


	if (IsGameOver()) Debug::Print("Game is Finished", Vector2(75, 90));
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
