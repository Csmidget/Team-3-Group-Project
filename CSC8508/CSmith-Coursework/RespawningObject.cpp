#include "RespawningObject.h"

#include "../Engine/Debug.h"

#include <sstream>
#include <iomanip>

using namespace NCL;
using namespace CSC8508;

RespawningObject::RespawningObject(Vector3 spawnPosition, string name, bool randomizeOrientation) : GameObject(name) {
	this->spawnPosition = spawnPosition;
	this->randomizeOrientation = randomizeOrientation;

	Respawn();
}

void RespawningObject::Respawn() {
	respawnTimer = 0.0f;

	transform.SetPosition(spawnPosition);

	if (physicsObject) {
		physicsObject->SetLinearVelocity(Vector3());
		physicsObject->SetAngularVelocity(Vector3());
	}

	if (!randomizeOrientation) {
		transform.SetOrientation({});
	}
	else {
		Quaternion newRot =
			Matrix3::Rotation((float)(rand() % 360), Vector3(1, 0, 0)) *
			Matrix3::Rotation((float)(rand() % 360), Vector3(0, 1, 0)) *
			Matrix3::Rotation((float)(rand() % 360), Vector3(0, 0, 1));

		transform.SetOrientation(newRot);
	}

	OnRespawn();
}

void RespawningObject::OnUpdate(float dt) {

	float posOffset = (prevPos - transform.GetPosition()).Length();

	//If an object is stationary (Or close enough) for enough time, it respawns.

	if (posOffset > 0.3f) {
		respawnTimer = 0.0f;
		prevPos = transform.GetPosition();
	}
	else {
		respawnTimer += dt;

		if (respawnTimer > 1.5f) {
			Respawn();
		}
	}
}

void RespawningObject::OnKill() {
	Respawn();
}

void RespawningObject::ObjectSpecificDebugInfo(int& currLine, float lineSpacing) const {
	std::stringstream stream;

	stream << std::fixed << std::setprecision(2);

	stream << "Spawn point: " << spawnPosition;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");
}



