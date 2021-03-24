#pragma once
#include"HingeComponent.h"
#include"Game.h"

#include"../Engine/Physics/PhysicsEngine/BulletWorld.h"

using namespace NCL;
using namespace CSC8508;

HingeComponent::HingeComponent(GameObject* object, Game* game, Maths::Vector3 point, Maths::Vector3 axis)
	:Component("HingeComponent", object)
{
	game->GetPhysics()->addhingeconstraint(object->GetPhysicsObject()->body, point, axis);
}


