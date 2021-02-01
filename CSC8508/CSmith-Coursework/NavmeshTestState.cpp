#include "NavmeshTestState.h"

#include "AIObject.h"
#include "DebugState.h"
#include "Game.h"
#include "Checkpoint.h"
#include "../Engine/GameWorld.h"


using namespace NCL::CSC8508;

NavmeshTestState::NavmeshTestState(Game* game) : GameState(game) {

	camera = game->GetWorld()->GetMainCamera();
	game->InitNavmeshWorld();
}

PushdownState::PushdownResult NavmeshTestState::OnUpdate(float dt, PushdownState** newState) {

	Debug::Print("Press 'Backspace' to return to the menu.", Vector2(45, 91), Debug::YELLOW, 1.2f);
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::BACK)) {
		return PushdownResult::Pop;
	}

	Debug::Print("Press 'P' to pause the game", Vector2(45, 94), Debug::YELLOW, 1.2f);
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P)) {
		game->SetPause(!game->IsPaused());
	}

	Debug::Print("Press 'Tab' to enter debug mode", Vector2(45, 97), Debug::YELLOW, 1.2f);
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::TAB)) {
		*newState = new DebugState(game);
		return PushdownResult::Push;
	}

	for (auto opponent : game->GetOpponents()) {
		if (game->GetGoal()->ReachedCheckpoint(opponent)) {
			opponent->Respawn();
		}
	}

	camera->UpdateCamera(dt);

	return PushdownResult::NoChange;

}

void NavmeshTestState::OnAwake() {

}

void NavmeshTestState::OnSleep() {

}