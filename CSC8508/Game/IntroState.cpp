#include "IntroState.h"
#include "PlayState.h"
#include "Game.h"
#include "../Engine/GameWorld.h"
#include "../Engine/Debug.h"
#include "../Engine/CollisionDetection.h"
#include "../../Common/Window.h"
#include "../../Common/Camera.h"

using namespace NCL;
using namespace CSC8508;


IntroState::IntroState(Game* game) {
	this->game = game;
	this->world = game->GetWorld();
	openCube = nullptr;
	exitCube = nullptr;
}

PushdownState::PushdownResult IntroState::OnUpdate(float dt, PushdownState** newState) {
	Debug::Print("Game of Team 3!", Vector2(42, 30));
	Debug::Print("Click to Start", Vector2(42, 50));
	Debug::Print("Click to Exit", Vector2(43, 70));

	if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {

		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			GameObject* hitObject = (GameObject*)closestCollision.node;
			if (hitObject == openCube) {
				*newState = new PlayState(game);
				return PushdownResult::Push;
			}
			if (hitObject == exitCube) {
				Window::DestroyGameWindow();
				return PushdownResult::Pop;
			}
		}
	}

	return PushdownResult::NoChange;
};

void IntroState::OnAwake() {
	game->InitIntroWorld();
	openCube = game->AddButtonToWorld(Vector3(0, 0, 0), Vector3(8, 2, 0), 0.0f, false, false);
	exitCube = game->AddButtonToWorld(Vector3(0, -10, 0), Vector3(8, 2, 0), 0.0f, false, false);
	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(false);
}

void IntroState::OnSleep() {
	Window::GetWindow()->ShowOSPointer(false);
	Window::GetWindow()->LockMouseToWindow(true);
}