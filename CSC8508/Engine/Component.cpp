#include "Component.h"
#include "GameObject.h"

using namespace NCL::CSC8508;

Component::Component(GameObject* object) {
	gameObject = object;
	transform = &object->GetTransform();
}