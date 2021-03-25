#define NOMINMAX
#include "PlayState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "DebugState.h"
#include "Game.h"
#include "GameStateManagerComponent.h"
#include "ScoreComponent.h"
#include "LocalNetworkPlayerComponent.h"
#include "NetworkPlayerComponent.h"
#include "PlayerComponent.h"
#include "../Engine/GameWorld.h"

using namespace NCL;
using namespace CSC8508;

PlayState::PlayState(Game* game, bool isNetworked) {

	this->isNetworked = isNetworked;
	this->levelID = 0;
	this->game = game;	
	levels = new std::string[LEVELCOUNT]{ "Level1.json" , "Level2.json", "Level3.json" };

	game->InitWorld(levels[levelID]);
	levelID++;

	if (isNetworked)
		InitSpawns();

	GameObject* scoreObject = new GameObject();
	game->GetWorld()->AddGameObject(scoreObject);
	scoreObject->AddComponent<ScoreComponent>();
	//scoreObject->AddComponent<TimeScoreComponent>(game, 1);

	gameStateManager = GameStateManagerComponent::instance;
}

PushdownState::PushdownResult PlayState::OnUpdate(float dt, PushdownState** newState) {

	Debug::Print("Press F1 to return to main menu", Vector2(1, 5));

	if (!isNetworked) {
		Debug::Print("Press P to pause", Vector2(1, 10));

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P)) {
			*newState = new PauseState(game);
			return PushdownResult::Push;
		}
	}
	else {
		Debug::Print("Press Tab to view scoreboard", Vector2(1, 10));
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::TAB)) {
			ScoreComponent::DisplayScoreboard(game);
		}
	}

	if (gameStateManager->IsPlayerFinished() || gameStateManager->IsGameFinished()) {
		*newState = new GameOverState(game, levelID == LEVELCOUNT, isNetworked);
		return PushdownResult::Push;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
		*newState = new DebugState(game);
		return PushdownResult::Push;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F1)) {
		return PushdownResult::Pop;
	}

	return PushdownResult::NoChange;
};

void PlayState::InitSpawns() {
	const Vector3 spawnOffsets[8]{ {0,0,0},{2,0,0},{0,0,2},{2,0,2},{2,0,-2},{-2,0,0},{0,0,-2},{-2,0,2} };

	auto networkPlayers = game->GetWorld()->GetObjectsWithComponent<NetworkPlayerComponent>();
	auto localPlayer = game->GetWorld()->GetObjectsWithComponent<PlayerComponent>()[0];

	for (auto player : networkPlayers) {
		auto comp = player->GetComponent<NetworkPlayerComponent>();
		auto pos = localPlayer->GetTransform().GetPosition() + spawnOffsets[comp->GetPlayerID()];
		player->GetTransform().SetPosition(pos);
		comp->SetTargetPosition(pos);
		player->SetIsActive(true);
	}

	auto localPlayerComp = game->GetWorld()->GetComponentOfType<LocalNetworkPlayerComponent>();
	if (localPlayerComp) {
		int id = localPlayerComp->GetLocalPlayerID();
		localPlayer->GetTransform().SetPosition(localPlayer->GetTransform().GetPosition() + spawnOffsets[id]);
	}

}

void PlayState::OnAwake() {
	if (isNetworked)
	{
		if (!gameStateManager->IsGameFinished()) return;

		game->InitWorld(levels[levelID]);

		InitSpawns();	
	}
	else
	{
		if (!gameStateManager->IsGameFinished()) return;
		game->InitWorld(levels[levelID]);
	}

	auto localPlayer = game->GetWorld()->GetComponentOfType<LocalNetworkPlayerComponent>();
	if (localPlayer)
		localPlayer->SetGameFinished(false);

	gameStateManager = GameStateManagerComponent::instance;
	levelID = std::min(++levelID, LEVELCOUNT);
	gameStateManager->SetLevelID(levelID);
}