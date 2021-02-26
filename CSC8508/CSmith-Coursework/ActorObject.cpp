#include "ActorObject.h" 

#include "Checkpoint.h"
#include "Game.h"

#include "../Engine/Debug.h"
#include "../Engine/GameWorld.h"

#include <sstream>
#include <iomanip>

using namespace NCL::CSC8508;

void ActorObject::OnUpdate(float dt) {
	for (auto timedConstraint : timedConstraints) {

		timedConstraint.second -= dt;

		if (timedConstraint.second <= 0.0f) {
			timedConstraints.erase(timedConstraint.first);
			game->GetWorld()->RemoveConstraint(timedConstraint.first, true);
		}
	}

	lastCollisionTimer += dt;

	OnUpdate(dt);
}

void ActorObject::AddTimedConstraint(Constraint* constraint, float time) {
	timedConstraints.emplace(constraint, time);
}

void ActorObject::Respawn() {

	//If we don't have a checkpoint assigned, fallback to default respawningObject behaviour
	if (!currentCheckpoint) {
		RespawningObject::Respawn();
		return;
	}

	//Select a random spawn point from the available spawnpoints at current checkpoint.
	SetRespawnPosition(currentCheckpoint->GetSpawnPoint());

	//Now respawn at that point.
	RespawningObject::Respawn();
}

void ActorObject::OnCollisionBegin(GameObject* otherObject) {

	//When we collide with collectables we don't want to reset the collision timer...
	if (otherObject->GetPhysicsObject() && otherObject->IsStatic())
		lastCollisionTimer = 0.0f;
}

void ActorObject::ObjectSpecificDebugInfo(int& currLine, float lineSpacing) const {

	RespawningObject::ObjectSpecificDebugInfo(currLine, lineSpacing);

	std::stringstream stream;

	stream << std::fixed << std::setprecision(2);

	stream << "Last Collision Timer: " << lastCollisionTimer;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");
}