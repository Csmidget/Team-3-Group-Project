#include "GameStateManager.h"

NCL::CSC8508::GameStateManager::GameStateManager(GameWorld* gameWorld)
{
	world = gameWorld;
	RefreshPlayerList();
}

void NCL::CSC8508::GameStateManager::Update(float dt)
{
	if (IsGameOver()) {
		//Go to next scene
	}
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
