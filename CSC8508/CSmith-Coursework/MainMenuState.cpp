#include "MainMenuState.h"

#include "PracticeState.h"
#include "KatamariState.h"
#include "NavmeshTestState.h"
#include "DebugState.h"
#include "RaceState.h"
#include "Game.h"

#include "../Engine/GameWorld.h"

using namespace NCL::CSC8508;

MainMenuState::MainMenuState(Game* game) : GameState(game) {
	selection = 0;
	world = game->GetWorld();
	raceOpponents = 1;
	katamariBoids = 8;
};

PushdownState::PushdownResult MainMenuState::OnUpdate(float dt, PushdownState** newState) {
	Camera* cam = world->GetMainCamera();
	Debug::Print("Main Menu", Vector2(5, 20), Debug::YELLOW,1.5f);
	Debug::Print("Practice", Vector2(5, 30), selection == 0 ? Debug::GREEN : Debug::WHITE,1.5f);
	Debug::Print("Versus", Vector2(5, 40), selection == 1 ? Debug::GREEN : Debug::WHITE, 1.5f);
	Debug::Print("Katamari", Vector2(5, 50), selection == 2 ? Debug::GREEN : Debug::WHITE,1.5f);
	Debug::Print("Navmesh Test", Vector2(5, 60), selection == 3 ? Debug::GREEN : Debug::WHITE, 1.5f);
	Debug::Print("Quit", Vector2(5, 70), selection == 4 ? Debug::GREEN : Debug::WHITE,1.5f);

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::TAB)) {
		*newState = new DebugState(game);
		return PushdownResult::Push;
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P)) {
		game->SetPause(!game->IsPaused());
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::UP)) {
		--selection;
		if (selection < 0)
			selection = 4;
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::DOWN)) {
		selection = (selection + 1) % 5;
	}

	if (selection == 1) {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::RIGHT)) {
			++raceOpponents;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::LEFT)) {
			--raceOpponents;
		}

		raceOpponents = max(1, min(14, raceOpponents));
		std::string opponentsText = "Opponents:  < " + std::to_string(raceOpponents) + " >";
		Debug::Print(opponentsText, Vector2(20, 40), Debug::YELLOW, 1.5f);
	}

	if (selection == 2) {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::RIGHT)) {
			++katamariBoids;
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::LEFT)) {
			--katamariBoids;
		}

		katamariBoids = max(4, min(20, katamariBoids));
		std::string opponentsText = "Boids:  < " + std::to_string((int)pow(katamariBoids,2)) + " >";
		Debug::Print(opponentsText, Vector2(20, 50), Debug::YELLOW, 1.5f);
	}


	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::RETURN)) {
		switch (selection) {
		case 0:
			*newState = new PracticeState(game);
			return PushdownResult::Push;
		case 1:
			*newState = new RaceState(game,raceOpponents);
			return PushdownResult::Push;
		case 2:
			*newState = new KatamariState(game,katamariBoids);
			return PushdownResult::Push;
		case 3:
			*newState = new NavmeshTestState(game);
			return PushdownResult::Push;
		case 4:
			return PushdownResult::Pop;
		}
	}
	cameraYaw += dt * 2;

	cam->SetPosition( Quaternion(Matrix4::Rotation(cameraYaw,Vector3(0,1,0))) * Vector3(0, 0,250) + Vector3(0,150,0));
	cam->SetPitch(-30);
	cam->SetYaw(cameraYaw);

	return PushdownResult::NoChange;
}

void MainMenuState::OnAwake() {
	cameraYaw = 0;
	game->InitPracticeWorld();
}