#include "GameStateManager.h"

NCL::CSC8508::GameStateManager::GameStateManager(GameWorld* gameWorld)
{
	world = gameWorld;
	RefreshPlayerList();
}

void NCL::CSC8508::GameStateManager::Update(float dt)
{

	if (clientPlayerID < players.size() && clientPlayerID >= 0) 
		Debug::Print("Score: " + std::to_string(players.at(clientPlayerID)->GetComponent<ScoreComponent>()->GetScore()), Vector2(75, 95));

	
	if (IsGameOver()) Debug::Print("Game is Finished", Vector2(75, 90));
		//Go to next scene
	
}

void NCL::CSC8508::GameStateManager::RefreshPlayerList()
{
	if (!world) return;
	players.clear();
	players = world->GetObjectsWithTag("Player");

	for (int i = 0; i < players.size(); i++) 
		players.at(i)->AddComponent<ScoreComponent>();
	

}

bool NCL::CSC8508::GameStateManager::IsGameOver()
{
	for (int i = 0; i < players.size(); i++)
		if (!players.at(i)->GetComponent<ScoreComponent>()->IsFinished())
			return false;

	return true;
}
