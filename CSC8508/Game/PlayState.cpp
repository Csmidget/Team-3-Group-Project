#include "PlayState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "DebugState.h"
#include "Game.h"
#include "../Engine/GameWorld.h"
#include "../Engine/Debug.h"
#include "GameStateManagerComponent.h"
#include "ScoreComponent.h"
#include "TimeScoreComponent.h"

using namespace NCL;
using namespace CSC8508;

PlayState::PlayState(Game* game) {
	this->game = game;

	game->InitWorld("AshmanTest.json");
	
	GameObject* scoreObject = new GameObject();
	game->GetWorld()->AddGameObject(scoreObject);
	scoreObject->AddComponent<ScoreComponent>();
	scoreObject->AddComponent<TimeScoreComponent>(game, 1);

	gameStateManager = game->GetWorld()->GetComponentOfType<GameStateManagerComponent>();
}

PushdownState::PushdownResult PlayState::OnUpdate(float dt, PushdownState** newState) {

	Debug::Print("Press P to pause", Vector2(1, 5));
	Debug::Print("Press F1 to return to main menu", Vector2(1, 10));

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P)) {
		*newState = new PauseState(game);
		return PushdownResult::Push;
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
		*newState = new DebugState(game);
		return PushdownResult::Push;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F1)) {
		return PushdownResult::Pop;
	}
	if (gameStateManager->IsGameFinished()) {
		*newState = new GameOverState(game);
		return PushdownResult::Over;
	}

	return PushdownResult::NoChange;
};

void PlayState::OnAwake() {

}