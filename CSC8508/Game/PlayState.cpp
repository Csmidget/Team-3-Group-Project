#include "PlayState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "Game.h"
#include "../Engine/GameWorld.h"
#include "../Engine/Debug.h"

using namespace NCL;
using namespace CSC8508;

PlayState::PlayState(Game* game) {
	this->game = game;
	game->InitWorld();
	
	GameStateManagerComponent* gameState = GetGameStateManager();
	if (!gameState) return;
	gameState->SetClientScore(&score);
	gameState->SetIsGameFinished(&isGameFinished);

}

GameStateManagerComponent* NCL::CSC8508::PlayState::GetGameStateManager() const
{
	GameObject* object = game->GetWorld()->GetObjectWithTag("GameStateManager");
	return object ? object->GetComponent<GameStateManagerComponent>() : nullptr;	
}

PushdownState::PushdownResult PlayState::OnUpdate(float dt, PushdownState** newState) {

	Debug::Print("Press P to pause", Vector2(1, 5));
	Debug::Print("Press F1 to return to main menu", Vector2(1, 10));

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P)) {
		*newState = new PauseState(game);
		return PushdownResult::Push;

	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F1)) {
		return PushdownResult::Pop;
	}
	if (isGameFinished) {

		*newState = new GameOverState(game, score);
		return PushdownResult::Over;
	}

	
		
	return PushdownResult::NoChange;
};

void PlayState::OnAwake() {

}