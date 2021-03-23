#include "GameOverState.h"
#include "../Engine/GameWorld.h"
#include "Game.h"
#include "CameraComponent.h"
#include"PlayerComponent.h"
#include <iostream>
#include "../Engine/Debug.h"
#include "GameTechRenderer.h"
#include "ScoreComponent.h"
#include"../Game/GameStateManagerComponent.h"

using namespace NCL;
using namespace CSC8508;

GameOverState::GameOverState(Game* game, bool isFinal, bool isNetworked) {

	this->game = game;
	this->isFinal = isFinal;
	this->isNetworked = isNetworked;
	this->timer = 5.0f;
	gameScore = ScoreComponent::instance ? ScoreComponent::instance->GetScore() : 0;
	gameStateManager = nullptr;
	spectatorCamera = nullptr;
}

PushdownState::PushdownResult GameOverState::OnUpdate(float dt, PushdownState** newState) {	
	
	PrintOutcome();
	UpdateCameraControls(dt);

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::R)) return PushdownResult::Top;
	
	if (isFinal)
		return PushdownResult::NoChange;
	
	if (isNetworked)
	{
		if (game->IsAllPlayersFinished())
			timer -= dt;
		game->getRenderer()->DrawString("Next Level: " + (int)timer, Vector2(30, 95), Vector4(1.0f, 1.0f, 0.0f, 0.0f), 20.0f);

		if (timer <= 0.0f) return PushdownResult::Pop;
	}
	else
	{
		game->getRenderer()->DrawString("Press N to go to Next Level! " + (int)timer, Vector2(30, 95), Vector4(1.0f, 1.0f, 0.0f, 0.0f), 20.0f);
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::N)) return PushdownResult::Pop;
	}

	return PushdownResult::NoChange;
}

void GameOverState::OnAwake() {

	spectatorCamera = CameraComponent::GetMain();
	gameStateManager = game->GetWorld()->GetComponentOfType<GameStateManagerComponent>();

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
	game->getRenderer()->DrawString("Play again", Vector2(35, 10), Vector4(1.0f, 1.0f, 0.0f, 0.0f), 35.0f);
	game->getRenderer()->DrawString(ScoreComponent::instance->GetScore() > 0 ? "You win" : "You lose",
									Vector2(35, 20), 
									ScoreComponent::instance->GetScore() > 0 ? Vector4(0.0f, 1.0f, 0.0f, 0.0f) : Vector4(1.0f, 0.0f, 0.0f, 0.0f),
									50.0f);
	game->getRenderer()->DrawString("Your score: " + std::to_string(ScoreComponent::instance->GetScore()),
									Vector2(25, 50),
									Vector4(1.0f, 1.0f, 0.0f, 0.0f),
									35.0f);
	game->getRenderer()->DrawString("Press R to return to Menu", Vector2(30, 100), Vector4(1.0f, 1.0f, 0.0f, 0.0f), 20.0f);
	
	auto networkPlayers = game->GetWorld()->GetComponentsOfType<NetworkPlayerComponent>();
	for (auto i = 0; i < networkPlayers.size(); i++)
	{
		game->getRenderer()->DrawString("Player " + std::to_string(i) , Vector2(10, 55 + (i * 5)), Vector4(1.0f, 1.0f, 0.0f, 0.0f), 35.0f);
		game->getRenderer()->DrawString("score: " + std::to_string(networkPlayers[i]->GetScore()), Vector2(15, 55 + (i * 5)), Vector4(1.0f, 1.0f, 0.0f, 0.0f), 35.0f);
		//player->GetScore();
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