#include "RaceState.h"

#include "AIObject.h"
#include "PlayerObject.h"
#include "DebugState.h"
#include "EndState.h"
#include "Game.h"
#include "Checkpoint.h"

#include "../Engine/GameWorld.h"

#include <algorithm>

using namespace NCL::CSC8508;

RaceState::RaceState(Game* game, int opponents) : GameState(game) {
	scoreTracker = 0.0f;

	game->InitRaceWorld(opponents);
	camera = game->GetWorld()->GetMainCamera();

	game->GetPlayerObject()->AddScore(500);
	for (auto opponent : game->GetOpponents()) {
		opponent->AddScore(500);
	}
}

PushdownState::PushdownResult RaceState::OnUpdate(float dt, PushdownState** newState) {

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

	auto opponents = game->GetOpponents();
	PlayerObject* player = game->GetPlayerObject();

	UpdateScores(dt, player, opponents);

	//Display current scores
	Debug::Print("Player: " + std::to_string(player->GetScore()), Vector2(2, 5),playerFinished ? Debug::YELLOW : Debug::WHITE);

	for (int i = 0; i < opponents.size(); ++i) {
		Debug::Print(opponents[i]->GetName() + ": " + std::to_string(opponents[i]->GetScore()),Vector2(2,8.0f + i*3.0f), opponents[i]->IsAsleep() ? Debug::YELLOW : Debug::WHITE);
	}

	bool opponentsFinished = CheckForFinishes(player,opponents);

	if (!playerFinished) {
		player->UpdateControls(camera);
	}
	else {
		camera->UpdateCamera(dt);
	}

	//Is the game over?
	if (playerFinished && opponentsFinished) {

		//Find where the player placed in the scoreboard.
		int playerPlacing = 0;
		for (auto opponent : opponents) {
			if (opponent->GetScore() > player->GetScore()) {
				++playerPlacing;
			}
			else {
				break;
			}
		}

		//Switch to endstate with either a victory or loss screen.
		std::string winOrLose = (playerPlacing == 0 ? "You win!" : "You Lose!");
		const std::string placings[15]{"first","second","third","fourth","fifth","sixth","seventh","eigth","ninth","tenth","eleventh","twelth","thirteenth","fourteenth","fifteenth" };

		*newState = new EndState(game, winOrLose, "With a score of " + std::to_string(player->GetScore()) + " you came in " + placings[playerPlacing]);
		return PushdownResult::Replace;

	}

	return PushdownResult::NoChange;
}

bool RaceState::CheckForFinishes(PlayerObject* player, std::vector<AIObject*>& opponents) {

	//Reached goal or ran out of score.
	if (!playerFinished && (game->GetGoal()->ReachedCheckpoint(player) || player->GetScore() <= 0)) {
		playerFinished = true;
	}

	bool opponentsFinished = true;
	for (auto opponent : opponents) {
		if (opponent->IsAsleep())
			continue;

		if (opponent->GetScore() <= 0) {
			opponent->SetAsleep(true);
		}
		else if (game->GetGoal()->ReachedCheckpoint(opponent)) {
			opponent->SetAsleep(true);
		}
		else {
			opponentsFinished = false;
		}
	}
	return opponentsFinished;
}

void RaceState::UpdateScores(float dt, PlayerObject* player, std::vector<AIObject*>& opponents) {

	if (!game->IsPaused()) {
		scoreTracker += dt;

		if (scoreTracker > 1) {
			//This will floor scoretracker
			int scoreIncrement = (int)scoreTracker;

			if (!playerFinished) {
				player->AddScore(-scoreIncrement * 10);
			}

			for (auto opponent : opponents) {
				if (!opponent->IsAsleep()) {
					opponent->AddScore(-scoreIncrement * 10);
				}
			}
			scoreTracker -= scoreIncrement;
		}
	}

	std::sort(opponents.begin(), opponents.end(), [&](AIObject* a, AIObject* b) {
		return a->GetScore() > b->GetScore();
	});
}
