#include "GameOverState.h"
#include "Game.h"
#include <iostream>
#include "../Engine/Debug.h"

using namespace NCL;
using namespace CSC8508;

PushdownState::PushdownResult GameOverState::OnUpdate(float dt, PushdownState** newState) {	
	
	PrintOutcome();
	
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::SPACE)) return PushdownResult::Top;

	return PushdownResult::NoChange;
}

void GameOverState::OnAwake() {

}

void GameOverState::PrintOutcome()
{
	Debug::Print("Play again", Vector2(30, 10));
	Debug::Print(gameScore > 0 ? "You win" : "You lose", Vector2(30, 30));
	Debug::Print("Your score:" + std::to_string(gameScore), Vector2(30, 50));
}
