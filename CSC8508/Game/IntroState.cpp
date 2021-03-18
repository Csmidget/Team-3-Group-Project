#include "IntroState.h"
#include "PlayState.h"
#include "Game.h"
#include "../Engine/GameWorld.h"
#include "../Engine/Debug.h"
#include "../Engine/CollisionDetection.h"
#include "../../Common/Window.h"
#include "../../Common/Camera.h"
#include "CameraComponent.h"

using namespace NCL;
using namespace CSC8508;


IntroState::IntroState(Game* game) {
	this->game = game;
	this->world = game->GetWorld();
	openCube1 = nullptr;
	openCube1 = nullptr;
	exitCube = nullptr;
	confrontation = false;
}

PushdownState::PushdownResult IntroState::OnUpdate(float dt, PushdownState** newState) {
	Debug::Print("Game of Team 3!", Vector2(42, 30));
	Debug::Print("Single Mode", Vector2(42, 50));
	Debug::Print("Confrontation", Vector2(42, 70));
	Debug::Print("Click to Exit", Vector2(43, 90));

	if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {

		Ray ray = CollisionDetection::BuildRayFromMouse(*CameraComponent::GetMain()->GetCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			GameObject* hitObject = (GameObject*)closestCollision.node;
			if (hitObject == openCube1) {
				*newState = new PlayState(game);
				return PushdownResult::Push;
			}
			if (hitObject == openCube2) {
				*newState = new PlayState(game);
				return PushdownResult::Push;
				confrontation = true;
			}
			if (hitObject == exitCube) {
				return PushdownResult::Pop;
			}
		}
	}
	return PushdownResult::NoChange;
};

void IntroState::OnAwake() {
	game->InitIntroWorld();
	openCube1 = game->AddButtonToWorld(Vector3(0, 0, 0), Vector3(8, 2, 0), 0.0f, false, false);
	openCube2 = game->AddButtonToWorld(Vector3(0, -10, 0), Vector3(8, 2, 0), 0.0f, false, false);
	exitCube = game->AddButtonToWorld(Vector3(0, -20, 0), Vector3(8, 2, 0), 0.0f, false, false);
	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(false);
}

void IntroState::OnSleep() {
	Window::GetWindow()->ShowOSPointer(false);
	Window::GetWindow()->LockMouseToWindow(true);
}