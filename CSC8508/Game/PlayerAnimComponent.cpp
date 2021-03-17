#include "PlayerAnimComponent.h"
#include "PlayerComponent.h"
#include "../Engine/GameObject.h"
using namespace NCL;
using namespace CSC8508;

void PlayerAnimComponent::Start() {
	playerComp = gameObject->GetComponent<PlayerComponent>();
}

void PlayerAnimComponent::Update(float dt) {
	
}
