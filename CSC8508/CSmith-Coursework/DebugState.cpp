#include "DebugState.h"
#include "Game.h"

#include "../Engine/GameWorld.h"
#include "../Engine/Debug.h"
#include "../../Common/Window.h"

using namespace NCL::CSC8508;

DebugState::DebugState(Game* game) : GameState(game) {
	world = game->GetWorld();
	camera = world->GetMainCamera();
	selectionMode = false;
	selectionObject = nullptr;
	selectionObjectColour = Vector4(1, 1, 1, 1);
}

DebugState::~DebugState() {

	//clear colour change if an object is selected;
	if (selectionObject) {
		selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));
	}

	Window::GetWindow()->ShowOSPointer(false);
	Window::GetWindow()->LockMouseToWindow(true);
}

PushdownState::PushdownResult DebugState::OnUpdate(float dt, PushdownState** newState) {

	if (selectionMode) {
		Debug::Print("Press Q to change to camera mode!", Vector2(5, 85));
	}
	else {
		Debug::Print("Press Q to change to selection mode!", Vector2(5, 85));

	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::TAB) || Window::GetKeyboard()->KeyPressed(KeyboardKeys::BACK)) {
		return PushdownResult::Pop;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {

		if (selectionMode) {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
			selectionMode = false;
		}
		else {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
			selectionMode = true;
		}
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P)) {
			game->SetPause(!game->IsPaused());
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
		game->SetUseGravity(!game->GetUseGravity());//Toggle gravity!
	}

	Debug::Print("1: Show/Hide QuadTree", Vector2(5, 90));
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NUM1)) {
		game->GetWorld()->FlipDisplayQuadTree();
	}

	if (game->GetUseGravity()) {
		Debug::Print("(G)ravity on", Vector2(5, 95));
	}
	else {
		Debug::Print("(G)ravity off", Vector2(5, 95));
	}

	//Display debug info for selected object
	if (selectionObject) {
		if (selectionObject->GetRenderObject()->GetColour() != Debug::GREEN) {
			selectionObjectColour = selectionObject->GetRenderObject()->GetColour();
			selectionObject->GetRenderObject()->SetColour(Debug::GREEN);
		}
		selectionObject->PrintDebugInfo();
	}

	if (!selectionMode) {
		camera->UpdateCamera(dt);
	}
	else {
		if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(selectionObjectColour);
				selectionObject = nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*camera);
			RayCollision closestCollision;
			if (world->Raycast(ray, closestCollision, true,true)) {
				selectionObject = (GameObject*)closestCollision.node;
				selectionObjectColour = selectionObject->GetRenderObject()->GetColour();
				selectionObject->GetRenderObject()->SetColour(Debug::GREEN);
			}
		}
	}

	return PushdownResult::NoChange;
}

void DebugState::OnSleep() {
}

void DebugState::OnAwake() {
}