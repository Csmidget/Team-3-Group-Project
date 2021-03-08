#include "GameOverState.h"
#include "Game.h"
#include <iostream>
#include "../Engine/Debug.h"

using namespace NCL;
using namespace CSC8508;

PushdownState::PushdownResult GameOverState::OnUpdate(float dt, PushdownState** newState) {
	g2->UpdateGame(dt);

	if (gameScore <= 0) {
		Debug::Print("Play again", Vector2(30, 10));
		Debug::Print("You lost", Vector2(30, 50));
	}
	else {
		Debug::Print("Play again", Vector2(30, 10));
		Debug::Print("You win", Vector2(30, 30));
		Debug::Print("Play again", Vector2(30, 10));
		Debug::Print("Your score:" + std::to_string(gameScore), Vector2(30, 50));
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::SPACE)) {
		return PushdownResult::Top;

	}


	return PushdownResult::NoChange;
}

void GameOverState::OnAwake() {
	g2 = new Game("2");
}