#include "GrideComponent.h"
#include "../Engine/Transform.h"
#include "../Engine/GameObject.h"
#include "../Engine/PhysicsObject.h"

using namespace NCL;
using namespace CSC8508;

GrideComponent::GrideComponent(GameObject* object) : Component("GrideComponent", object) {

};

void GrideComponent::OnCollisionStay(GameObject* other) {
	if (other->HasTag("Gride"))
	{
		other->SetIsActive(false);
	}
}
