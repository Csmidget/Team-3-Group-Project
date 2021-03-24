#include "IntroState.h"
#include "PlayState.h"
#include "LobbyState.h"
#include "Game.h"
#include "CameraComponent.h"

#include "../Engine/GameWorld.h"
#include "../Engine/Debug.h"
#include "../Engine/CollisionDetection.h"
#include "../../Common/Window.h"

using namespace NCL;
using namespace CSC8508;


IntroState::IntroState(Game* game) {
	this->game = game;
	this->world = game->GetWorld();
	singleCube = nullptr;
	clientCube = nullptr;
	serverCube = nullptr;
	exitCube = nullptr;
	confrontation = false;
}

PushdownState::PushdownResult IntroState::OnUpdate(float dt, PushdownState** newState) {
	Debug::Print("Game of Team 3!", Vector2(42, 20));
	Debug::Print("Single Mode", Vector2(43.5, 34.5));
	Debug::Print("Multi - Client", Vector2(42, 50.5));
	Debug::Print("Multi - Server", Vector2(42, 66.5));
	Debug::Print("Click to Exit", Vector2(43, 82.5));

	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::LEFT)) {

		Ray ray = CollisionDetection::BuildRayFromMouse(*CameraComponent::GetMain()->GetCamera());

		RayCollision closestCollision;
		if (world->Raycast(ray, closestCollision, true)) {
			GameObject* hitObject = (GameObject*)closestCollision.node;
			if (hitObject == singleCube) {
				*newState = new PlayState(game);
				return PushdownResult::Push;
			}
			if (hitObject == clientCube) {
				*newState = new LobbyState(game,true);
				return PushdownResult::Push;
				confrontation = true;
			}
			if (hitObject == serverCube) {
				*newState = new LobbyState(game,false);
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
	singleCube = game->AddButtonToWorld(Vector3(0, 8, 0), Vector3(8, 2, 0), 0.0f, false);
	clientCube = game->AddButtonToWorld(Vector3(0, 0, 0), Vector3(8, 2, 0), 0.0f, false);
	serverCube = game->AddButtonToWorld(Vector3(0, -8, 0), Vector3(8, 2, 0), 0.0f, false);
	exitCube = game->AddButtonToWorld(Vector3(0, -16, 0), Vector3(8, 2, 0), 0.0f, false);
	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(false);
}

void IntroState::OnSleep() {
	Window::GetWindow()->ShowOSPointer(false);
	Window::GetWindow()->LockMouseToWindow(true);
}