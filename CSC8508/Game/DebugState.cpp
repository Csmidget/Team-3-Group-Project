#include "DebugState.h"
#include "Game.h"
#include "PlayerComponent.h"
#include "CameraComponent.h"
#include "../Engine/GameObject.h"
#include "../Engine/GameWorld.h"

using namespace NCL;
using namespace CSC8508;

DebugState::DebugState(Game* game) {
	this->game = game;
	this->oldMain = nullptr;
	this->debugCamera = nullptr;
}

PushdownState::PushdownResult DebugState::OnUpdate(float dt, PushdownState** newState) {

	if (!selectionMode) {
		Debug::Print("Hit Q to enter selection mode", Vector2(2, 90));
		UpdateCameraControls(dt);
	}
	else {
		Debug::Print("Hit Q to enter camera mode", Vector2(2, 90));

		if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::LEFT)) {
			const Camera& cam = (*debugCamera->GetCamera());
			auto r = CollisionDetection::BuildRayFromMouse(cam);
			auto object = game->Raycast(r.GetPosition(), r.GetPosition() + r.GetDirection() * 1000);

			if (object) {
				selectedObject = object;
				object->GetRenderObject()->SetAnimation(nullptr);
			}
			else
				selectedObject = nullptr;
		}
	}

	//Safety check to ensure we return the correct main camera after finishing in debug mode.
	if (CameraComponent::GetMain() != debugCamera) {
		oldMain = CameraComponent::GetMain();
		CameraComponent::SetMain(debugCamera);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		selectionMode = !selectionMode;
		Window::GetWindow()->ShowOSPointer(selectionMode);
		Window::GetWindow()->LockMouseToWindow(!selectionMode);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
		return PushdownResult::Pop;
	}

	if (selectedObject) {
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::K))
			selectedObject->GetRenderObject()->SetAnimation(game->GetResourceManager()->LoadAnimation("role_T.anm"));
		DisplayDebugInfo();
	}

	return PushdownResult::NoChange;
}

void DebugState::DisplayDebugInfo() {
	const int maxLines = 20;
	
	std::vector<std::string> objectDebugInfo;
	objectDebugInfo = selectedObject->DebugInfo();

	//Figure how far through our debug info we can scroll.
	int maxScroll = std::max(0, (int)objectDebugInfo.size() - maxLines);
	std::clamp(debugInfoScroll, 0, maxScroll);

	int count = std::min(maxLines, (int)objectDebugInfo.size() - debugInfoScroll);

	for (int i = 0; i < count; i++) {
		Debug::Print(objectDebugInfo[debugInfoScroll + i], Vector2(2,4 + 4 * i));
	}
}

void DebugState::UpdateCameraControls(float dt) {
	//Update the mouse by how much
	float pitch = debugCamera->GetPitch() - Window::GetMouse()->GetRelativePosition().y;
	float yaw = debugCamera->GetYaw() - Window::GetMouse()->GetRelativePosition().x;

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	debugCamera->SetPitch(pitch);
	debugCamera->SetYaw(yaw);

	float frameSpeed = 100 * dt;

	Vector3 position = debugCamera->GetPosition();
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * frameSpeed;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * frameSpeed;
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * frameSpeed;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * frameSpeed;
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::SHIFT)) {
		position.y += frameSpeed;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::SPACE)) {
		position.y -= frameSpeed;
	}

	debugCamera->SetPosition(position);
}

void DebugState::OnAwake() {

	oldMain = CameraComponent::GetMain();
	debugCameraObject = new GameObject();
	debugCamera = debugCameraObject->AddComponent<CameraComponent>(true);
	debugCamera->SetPosition(oldMain->GetCamera()->GetPosition());
	debugCamera->SetPitch(oldMain->GetPitch());
	debugCamera->SetYaw(oldMain->GetYaw());
	debugCamera->SetFarPlane(1000.0f);

	Window::GetWindow()->ShowOSPointer(selectionMode);
	Window::GetWindow()->LockMouseToWindow(!selectionMode);

	playerComponents = game->GetWorld()->GetComponentsOfType<PlayerComponent>();

	for (int i = playerComponents.size() - 1; i >= 0; --i) {
		if (!playerComponents[i]->IsEnabled())
			playerComponents.erase(playerComponents.begin() + i);

		playerComponents[i]->SetEnabled(false);
	}
}

void DebugState::OnSleep() {
	for (auto comp : playerComponents) {
		comp->SetEnabled(true);
	}

	CameraComponent::SetMain(oldMain);
	delete debugCameraObject;


	Window::GetWindow()->ShowOSPointer(false);
	Window::GetWindow()->LockMouseToWindow(true);
}