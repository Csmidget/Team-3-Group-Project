#include "IntroState.h"
#include "PlayState.h"
#include "Game.h"
#include "../Engine/Debug.h"

using namespace NCL;
using namespace CSC8508;


PushdownState::PushdownResult IntroState::OnUpdate(float dt, PushdownState** newState) {
	Debug::Print("Click the Cube to Start game", Vector2(1, 5));
	switch (g0->GetOpenOrExit()) {
	case 0:
		g0->UpdateIntroGame(dt);
		return PushdownResult::NoChange;
	case 1:
		*newState = new PlayState();
		return PushdownResult::Push;
		break;
	case 2:
		Window::DestroyGameWindow();
		return PushdownResult::Pop;
		break;
		/*		case 5:
					addAi = true;
					*newState = new GameScreen();
					return PushdownResult::Push;
					break;*/
	}
};

void IntroState::OnAwake() {
	g0 = new Game("0");
}