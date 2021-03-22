#include "ScoreComponent.h"
#include "../Engine/GameObject.h"
#include "BonusComponent.h"
#include "RingComponent.h"

#include <algorithm>

using namespace NCL;
using namespace CSC8508;

ScoreComponent* ScoreComponent::instance = nullptr;

ScoreComponent::ScoreComponent(GameObject* object) : Component("ScoreComponent", object)
{
	//TODO: DESTROY IF INSTANCE ALREADY EXISTS
	if (!instance)
		instance = this;

	gameObject->SetPersistence(true);

	hasFinished = false;
	score = 0;
}

ScoreComponent::~ScoreComponent()
{
	if (instance = this)
		instance = nullptr;
}

void ScoreComponent::AddScore(int val)
{
	this->score = std::max(0, (score + val));
}

void ScoreComponent::OnCollisionBegin(GameObject* otherObject)
{
	if (!otherObject->IsActive()) return;

	if (otherObject->HasTag("Bonus")) {
		score += otherObject->GetComponent<BonusComponent>()->GetBonus();
		otherObject->OnKill();
		return;
	}

	if (otherObject->HasTag("Ring"))
	{
		score += otherObject->GetComponent<RingComponent>()->GetBonus();
		otherObject->OnKill();
		return;
	}

	if (otherObject->HasTag("Goal")) hasFinished = true;
}
