#include "GameOverState.h"
#include "../Engine/GameWorld.h"
#include "Game.h"
#include "CameraComponent.h"
#include"PlayerComponent.h"
#include <iostream>
#include "../Engine/Debug.h"
#include "GameTechRenderer.h"

using namespace NCL;
using namespace CSC8508;

GameOverState::GameOverState(Game* game, int score) {

	this->game = game;
	gameScore = score;
	spectatorCamera = nullptr;
}

PushdownState::PushdownResult GameOverState::OnUpdate(float dt, PushdownState** newState) {	
	
	PrintOutcome();
	UpdateCameraControls(dt);

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::R)) return PushdownResult::Top;

	return PushdownResult::NoChange;
}

void GameOverState::OnAwake() {

	spectatorCamera = CameraComponent::GetMain();

	auto playerComponents = game->GetWorld()->GetComponentsOfType<PlayerComponent>();
	auto playerObject = game->GetWorld()->GetObjectsWithTag("Player");

	for (int i = 0; i<=playerObject.size() - 1; i++)
		playerObject[i]->SetIsActive(false);
	
	for (int i = playerComponents.size() - 1; i >= 0; --i) {
		if (!playerComponents[i]->IsEnabled())
			playerComponents.erase(playerComponents.begin() + i);

		playerComponents[i]->SetEnabled(false);
	}
}

void GameOverState::PrintOutcome()
{
	Debug::Print("Play again", Vector2(30, 10));
	Debug::Print(gameScore > 0 ? "You win" : "You lose", Vector2(30, 30));
	Debug::Print("Your score:" + std::to_string(gameScore), Vector2(30, 50));
	Debug::Print("Press R to return to Menu", Vector2(30, 70));

	game->getRenderer()->DrawString("Play again", Vector2(30, 10), Vector4(1.0f, 0.0f, 0.0f, 0.0f), 50.0f);
	game->getRenderer()->DrawString(gameScore > 0 ? "You win" : "You lose", Vector2(30, 30), Vector4(1.0f, 0.0f, 0.0f, 0.0f), 50.0f);
	//game->getRenderer()->DrawString("Your score:" + std::to_string(gameScore), Vector2(30, 50), gameScore > 0 ? Vector4(1.0f, 0.0f, 0.0f, 0.0f), 50.0f);
	game->getRenderer()->DrawString("Play again", Vector2(30, 10), Vector4(1.0f, 0.0f, 0.0f, 0.0f), 50.0f);
	
	vector<GameObject*> players = game->GetWorld()->GetObjectsWithTag("Player");

	for (auto i : players)
	{

	}

}

void GameOverState::UpdateCameraControls(float dt) {
	//Update the mouse by how much
	float pitch = spectatorCamera->GetPitch() - Window::GetMouse()->GetRelativePosition().y;
	float yaw = spectatorCamera->GetYaw() - Window::GetMouse()->GetRelativePosition().x;

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = min(pitch, 90.0f);
	pitch = max(pitch, -90.0f);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	spectatorCamera->SetPitch(pitch);
	spectatorCamera->SetYaw(yaw);

	float frameSpeed = 100 * dt;

	Vector3 position = spectatorCamera->GetPosition();
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

	spectatorCamera->SetPosition(position);
}