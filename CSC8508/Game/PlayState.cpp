#include "PlayState.h"
#include "PauseState.h"
#include "Game.h"
#include "../Engine/Debug.h"

using namespace NCL;
using namespace CSC8508;

PushdownState::PushdownResult PlayState::OnUpdate(float dt, PushdownState** newState) {

	Debug::Print("Press P to pause", Vector2(1, 5));
	Debug::Print("Press F1 to return to main menu", Vector2(1, 10));

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::P)) {
		*newState = new PauseState();
		return PushdownResult::Push;

	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::F1)) {
		return PushdownResult::Pop;

	}
	// function for game over £¨ÅÐ¶ÏÓÎÏ·ÊÇ·ñ½áÊø£©
/*		if (g1->GetFinal() || g1->GetScore() <= 0) {
			gameScore = g1->GetScore();
			*newState = new OverScreen();
			return PushdownResult::Over;
		}*/
	else {
		g1->UpdateGame(dt);
	}
	return PushdownResult::NoChange;
};

void PlayState::OnAwake() {
	g1 = new Game("1");
}