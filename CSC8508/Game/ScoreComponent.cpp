#include "ScoreComponent.h"
#include "../Engine/GameObject.h"

NCL::CSC8508::ScoreComponent::ScoreComponent(GameObject* object) : Component(object)
{
	score = 0;
}

void NCL::CSC8508::ScoreComponent::OnCollisionBegin(GameObject* otherObject)
{
	if (otherObject->HasTag("Bonus")) {
		//dynamic cast bonus type to get bonus
		//score += otherObject.getcomponent<Bonus>.getbonus
	}
}
