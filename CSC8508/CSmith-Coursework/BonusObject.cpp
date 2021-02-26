#include "BonusObject.h"
#include "ActorObject.h"

using namespace NCL::CSC8508;

void BonusObject::OnUpdate(float dt) {
	if (popped) {
		respawnTimer -= dt;

		if (respawnTimer <= 0.0f) {
			popped = false;
			boundingVolume->SetActive(true);
			transform.SetScale(savedScale);
			savedScale = Vector3();
		}
	}

	transform.SetOrientation(transform.GetOrientation() * Matrix4::Rotation(dt * 30.0f, Vector3(0, 1, 0)));
}

void BonusObject::OnCollisionBegin(GameObject* otherObject) {
	if (!popped && otherObject->HasTag("actor")) {
		popped = true;
		respawnTimer = respawnDelay;
		savedScale = transform.GetScale();
		transform.SetScale(Vector3(0, 0, 0));
		ApplyBonus((ActorObject*)otherObject);
	}
}