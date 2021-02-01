#include "PracticeState.h"

#include "PlayerObject.h"
#include "DebugState.h"
#include "EndState.h"
#include "Game.h"
#include "Checkpoint.h"

#include "../Engine/CollisionDetection.h"
#include "../Engine/GameWorld.h"

using namespace NCL::CSC8508;

PracticeState::PracticeState(Game* game) : GameState(game) {
	camera = game->GetWorld()->GetMainCamera();
	game->InitPracticeWorld();
	scoreTracker = 0.0f;
	game->GetPlayerObject()->AddScore(1000);

}

PushdownState::PushdownResult PracticeState::OnUpdate(float dt, PushdownState** newState) {

	Debug::Print("Press 'Backspace' to return to the menu.", Vector2(45, 91), Debug::YELLOW, 1.2f);
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::BACK)) {
		return PushdownResult::Pop;
	}

	Debug::Print("Press 'P' to pause the game", Vector2(45, 94), Debug::YELLOW, 1.2f);
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P)) {
		game->SetPause(!game->IsPaused());
	}

	Debug::Print("Press 'Tab' to enter debug mode",Vector2(45,97),Debug::YELLOW,1.2f);
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::TAB)) {
		*newState = new DebugState(game);
		return PushdownResult::Push;
	}

	PlayerObject* player = game->GetPlayerObject();
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM1)) {
		player->GetTransform().SetPosition(Vector3(-100, 10, 100));
	}
	else if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM2)) {
		player->GetTransform().SetPosition(Vector3(-100, 10, -100));
	}
	else if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM3)) {
		player->GetTransform().SetPosition(Vector3(100, 110.0f, -100));
	}
	
	scoreTracker += dt;

	if (scoreTracker > 1) {
		//This will floor the value of scoreTracker.
		int scoreIncrement = (int)scoreTracker;
		player->AddScore(-scoreIncrement * 10);
		scoreTracker -= scoreIncrement;
	}

	Debug::Print("Score: " + std::to_string(player->GetScore()), Vector2(5, 5));

	player->UpdateControls(camera);

	//Check if player is colliding with the goal bounds
	if (game->GetGoal()->ReachedCheckpoint(player)) {
		*newState = new EndState(game, "You Win!", "Score: " + std::to_string(player->GetScore()));
		return PushdownResult::Replace;
	}

	//Score hits 0
	if (player->GetScore() <= 0) {
		*newState = new EndState(game, "You Lose!", "You ran out of score.");
		return PushdownResult::Replace;
	}

	return PushdownResult::NoChange;
}

void PracticeState::OnAwake() {

}

void PracticeState::OnSleep() {

}