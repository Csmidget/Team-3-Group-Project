#include "ScoreComponent.h"
#include "../Engine/GameObject.h"
#include "BonusComponent.h"

NCL::CSC8508::ScoreComponent::ScoreComponent(GameObject* object) : Component(object)
{
	hasFinished = false;
	score = 0;
}

void NCL::CSC8508::ScoreComponent::OnCollisionBegin(GameObject* otherObject)
{
	if (!otherObject->IsActive()) return;

	if (otherObject->HasTag("Bonus")) {
		score += otherObject->GetComponent<BonusComponent>()->GetBonus();
		otherObject->OnKill();
		return;
	}

	if (otherObject->HasTag("Goal")) hasFinished = true;
	
}
