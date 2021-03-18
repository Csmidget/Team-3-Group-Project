#include "PlayerAnimComponent.h"
#include "PlayerComponent.h"
#include "../Engine/GameObject.h"
#include "../../Common/MeshAnimation.h"
#include "Game.h"
using namespace NCL;
using namespace CSC8508;

PlayerAnimComponent::PlayerAnimComponent(GameObject* object, Game* game) : Component("PlayerAnim", object) {

	auto rm = game->GetResourceManager();
	idleAnim = rm->LoadAnimation("");
	runAnim = rm->LoadAnimation("run.anm");
	jumpAnim = rm->LoadAnimation("jump.anm");
}

void PlayerAnimComponent::Start() {
	playerComp = gameObject->GetComponent<PlayerComponent>();
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
