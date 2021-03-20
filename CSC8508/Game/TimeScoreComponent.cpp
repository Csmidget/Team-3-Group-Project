#include"TimeScoreComponent.h"

using namespace NCL;
using namespace CSC8508;

TimeScoreComponent::TimeScoreComponent(GameObject* object)
	: Component("TimeScoreComponent", object)
{
	timeScore = 0.0f;
}

void TimeScoreComponent::Update(float dt)
{
	timeScore += dt;
}

int TimeScoreComponent::GetScore() const
{
	return (int)timeScore;
}


