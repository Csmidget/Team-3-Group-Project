#include "BonusComponent.h"
#include "../Engine/GameObject.h"


NCL::CSC8508::BonusComponent::BonusComponent(GameObject* object, int bonus) : Component(object)
{
	this->bonus = bonus;
	
}
