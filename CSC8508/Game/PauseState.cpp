#include "PauseState.h"
#include "Game.h"
#include "../Engine/Debug.h"

using namespace NCL;
using namespace CSC8508;

PushdownState::PushdownResult PauseState::OnUpdate(float dt, PushdownState** newState) {
	Debug::Print("Press K to return", Vector2(0, 5));
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::K)) {
		return PushdownResult::Pop;
	}

	return PushdownResult::NoChange;
}

void PauseState::OnAwake() {

};