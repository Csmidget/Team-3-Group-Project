#include "GameOverState.h"
#include "Game.h"
#include "CameraComponent.h"
#include "PlayerComponent.h"
#include "GameTechRenderer.h"
#include "ScoreComponent.h"
#include "GameStateManagerComponent.h"

#include "../Engine/GameWorld.h"
#include "../../Common/Vector3.h"

#include <iostream>

using namespace NCL;
using namespace CSC8508;
using namespace Maths;

GameOverState::GameOverState(Game* game, bool isFinal, bool isNetworked) {

	this->game = game;
	this->isFinal = isFinal;
	this->isNetworked = isNetworked;
	this->allFinished = false;
	this->timer = 5.0f;
	gameScore = ScoreComponent::instance ? ScoreComponent::instance->GetScore() : 0;
	gameStateManager = nullptr;
	spectatorCamera = nullptr;
}

PushdownState::PushdownResult GameOverState::OnUpdate(float dt, PushdownState** newState) {	
	
	ScoreComponent::DisplayScoreboard(game);
	UpdateCameraControls(dt);

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::R)) return PushdownResult::Top;
	
	std::string completeString = isFinal ? "Game Completed!" : "Level Complete";

	game->getRenderer()->DrawString(completeString, Vector2(38, 10), Vector4(1.0f, 1.0f, 1.0f, 0.0f), 20.0f);
	game->getRenderer()->DrawString("Press R to return to Menu", Vector2(34, 99), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 15.0f);

	if (isFinal)
		return PushdownResult::NoChange;

	if (isNetworked)
	{
		allFinished = allFinished || gameStateManager->IsGameFinished();

		if (allFinished)
		{
			timer -= dt;
			game->getRenderer()->DrawString("Next Level: " + std::to_string((int)timer), Vector2(43, 95), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 15.0f);
		}
		else
			game->getRenderer()->DrawString("Waiting for other players!", Vector2(35, 95), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 15.0f);

		if (timer <= 0.0f) 
				return PushdownResult::Pop;
	}
	else
	{
		game->getRenderer()->DrawString("Press N to go to Next Level! " + (int)timer, Vector2(35, 95), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 15.0f);
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::N)) return PushdownResult::Pop;
	}

	return PushdownResult::NoChange;
}

void GameOverState::OnAwake() {

	spectatorCamera = CameraComponent::GetMain();
	gameStateManager = GameStateManagerComponent::instance;

	//If the player didn't finish, subtract from their score.
	if (!gameStateManager->IsPlayerFinished() && ScoreComponent::instance)
		ScoreComponent::instance->AddScore(-50);

	auto playerObject = game->GetWorld()->GetObjectsWithComponent<PlayerComponent>();
	
	for (int i = 0; i<=playerObject.size() - 1; i++)
		playerObject[i]->SetIsActive(false);
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

	Maths::Vector3 position = spectatorCamera->GetPosition();
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