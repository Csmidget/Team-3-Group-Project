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
}

PushdownState::PushdownResult IntroState::OnUpdate(float dt, PushdownState** newState) {
	Debug::Print("Click the Cube to Start game", Vector2(1, 5));

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
	openCube = this->game->InitOpenCube();

	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(false);
}

void IntroState::OnSleep() {
	Window::GetWindow()->ShowOSPointer(false);
	Window::GetWindow()->LockMouseToWindow(true);
}