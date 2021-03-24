#include "PlayerAnimComponent.h"
#include "Game.h"

#include "../Engine/GameObject.h"
#include "../../Common/MeshAnimation.h"
#include "../../Common/ResourceManager.h"

using namespace NCL;
using namespace CSC8508;

PlayerAnimComponent::PlayerAnimComponent(GameObject* object, Game* game) : Component("PlayerAnim", object) {

	auto rm = game->GetResourceManager();
	idleAnim = rm->LoadAnimation("idle.anm");
	runAnim = rm->LoadAnimation("run.anm");
	jumpAnim = rm->LoadAnimation("midair.anm");
	currentState = PlayerMovementState::IDLE;
	playerComp = nullptr;
}

void PlayerAnimComponent::Start() {
	playerComp = gameObject->GetComponent<PlayerComponent>();

	currentState = playerComp->GetCurrentMovementState();
	RenderObject* ro = gameObject->GetRenderObject();

	switch (currentState) {
	case PlayerMovementState::IDLE:
		ro->SetAnimation(idleAnim);
		break;
	case PlayerMovementState::JUMP_ONE:
	case PlayerMovementState::JUMP_TWO:
		ro->SetAnimation(jumpAnim);
		break;
	case PlayerMovementState::WALKING:
		ro->SetAnimation(runAnim);
		break;
	}
}

void PlayerAnimComponent::Update(float dt) {
	
	PlayerMovementState newState = playerComp->GetCurrentMovementState();
	if (newState != currentState) {
		RenderObject* ro = gameObject->GetRenderObject();

		switch (newState) {
			case PlayerMovementState::IDLE :
				ro->SetAnimation(idleAnim);
				break;
			case PlayerMovementState::JUMP_ONE:
			case PlayerMovementState::JUMP_TWO:
				ro->SetAnimation(jumpAnim);
				break;
			case PlayerMovementState::WALKING:
				ro->SetAnimation(runAnim);
				break;
		}

		currentState = newState;
	}
}
