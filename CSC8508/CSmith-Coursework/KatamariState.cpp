#include "KatamariState.h"

#include "PlayerObject.h"
#include "AIObject.h"
#include "DebugState.h"
#include "EndState.h"
#include "Game.h"
#include "Checkpoint.h"
#include "BoidSwarm.h"
#include "ObjectFactory.h"

#include "../Engine/CollisionDetection.h"

using namespace NCL::CSC8508;

KatamariState::KatamariState(Game* game, int boidLayers) : GameState(game) {

	camera = game->GetWorld()->GetMainCamera();
	game->InitKatamariWorld();

	boidSwarm = new BoidSwarm(game);
	Vector3 start(-40.0f, 2.0f, -40.0f);
	float step = 40.0f / (boidLayers / 2);

	for (int x = 0; x < boidLayers; x++) {
		for (int y = 0; y < boidLayers; y++) {
			game->GetObjectFactory()->CreateBoid(game, &boidSwarm, start + Vector3(x* step,0,y* step));
		}
	}

	requiredBoids = (int)pow(boidLayers, 2) / 5;

	boidSwarm->Avoid(game->GetPlayerObject());

	for (auto opponent : game->GetOpponents()) {
		boidSwarm->Avoid((GameObject*)opponent);
	}
}

KatamariState::~KatamariState() {
	delete boidSwarm;
}

PushdownState::PushdownResult KatamariState::OnUpdate(float dt, PushdownState** newState) {

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

	Debug::Print("Catch balls and be the first to " + std::to_string(requiredBoids) + " points!", Vector2(15, 5), Vector4(1, 1, 0, 1), 1.5f);

	int playerScore = game->GetPlayerObject()->GetScore();

	if (playerScore >= requiredBoids) {
		*newState = new EndState(game, "You win!", "You were the first to reach " + std::to_string(requiredBoids) + " points!");
		return PushdownResult::Replace;
	}

	Debug::Print("Player: " + std::to_string(playerScore), Vector2(2, 20), Debug::YELLOW, 1.1f);
	auto opponents = game->GetOpponents();
	for (int i = 0; i < opponents.size(); ++i) {
		Debug::Print(opponents[i]->GetName() + ": " + std::to_string(opponents[i]->GetScore()), Vector2(2, 23.0f + i * 3.0f),Debug::YELLOW,1.1f);

		if (opponents[i]->GetScore() >= requiredBoids) {
			*newState = new EndState(game, "You lose!", "One of your opponents caught " + std::to_string(requiredBoids) + " first!");
			return PushdownResult::Replace;
		}
	}

	//camera->UpdateCamera(dt);
	game->GetPlayerObject()->UpdateControls(camera);

	boidSwarm->Update();

	return PushdownResult::NoChange;
}

void KatamariState::OnAwake() {

}

void KatamariState::OnSleep() {

}