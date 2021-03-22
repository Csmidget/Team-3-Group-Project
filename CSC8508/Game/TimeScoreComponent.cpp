#include"TimeScoreComponent.h"
#include "ScoreComponent.h"
#include "Game.h"
#include "../Engine/GameWorld.h"
using namespace NCL;
using namespace CSC8508;

TimeScoreComponent::TimeScoreComponent(GameObject* object, Game* game, int strength)
	: Component("TimeScoreComponent", object)
{
	timer = 0.0f;
	this->strength = strength;
}

void TimeScoreComponent::Update(float dt)
{
	timer += dt;

	if (timer > 1.0f) {
		ScoreComponent::instance->AddScore(strength);
		timer -= 1.0f;
	}
}


