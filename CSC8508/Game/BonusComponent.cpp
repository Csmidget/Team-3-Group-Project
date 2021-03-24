#include "BonusComponent.h"

NCL::CSC8508::BonusComponent::BonusComponent(GameObject* object, int bonus) : Component("BonusComponent", object)
{
	this->bonus = bonus;
}
