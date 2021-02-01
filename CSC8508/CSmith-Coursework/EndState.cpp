#include "EndState.h"

#include "Game.h"
#include "DebugState.h"

#include "../Engine/Debug.h"

using namespace NCL::CSC8508;

PushdownState::PushdownResult EndState::OnUpdate(float dt, PushdownState** newState) {

	Debug::Print(mainText, Vector2(15, 30),Debug::YELLOW,1.5f);
	Debug::Print(subText, Vector2(15, 40), Debug::YELLOW, 1.5f);

	Debug::Print("Press space to return to the main menu.", Vector2(15, 60), Debug::YELLOW, 1.5f);

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::TAB)) {
		*newState = new DebugState(game);
		return PushdownResult::Push;
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P)) {
		game->SetPause(!game->IsPaused());
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE)) {
		return PushdownResult::Pop;
	}

	return PushdownResult::NoChange;
}


void EndState::OnAwake() {
	game->SetPause(true);
}

void EndState::OnSleep() {
	game->SetPause(false);
}