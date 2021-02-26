#include "BoidObject.h"
#include "ActorObject.h"
#include "BoidSwarm.h"
#include "Game.h"

#include "../Engine/Debug.h"
#include "../Engine/GameWorld.h"

#include <sstream>
#include <iomanip>

using namespace NCL::CSC8508;

BoidObject::BoidObject(Game* game, BoidSwarm** swarm, Vector3 respawnPosition) : RespawningObject(respawnPosition,"boid") {
	this->swarm = swarm;
	this->game = game;

	tags.push_back("boid");

	if (*swarm)
		(*swarm)->AddBoid(this);
}

BoidObject::~BoidObject() {
	if ((*swarm))
		(*swarm)->RemoveBoid(this);
}

void BoidObject::OnUpdate(float dt) {

	const float speed = 0.8f;

	if (!(*swarm))
		return;

	Vector3 sumofPoints = (*swarm)->SumOfPoints();
	Vector3 currPos = transform.GetPosition();

	//Move towards centre of swarm
	Vector3 swarmCentreDir;
	{
		const float swarmCentreStrength = 0.005f;

		sumofPoints -= currPos;

		//Find the average position of all other boids in the swarm.
		Vector3 targetPos = sumofPoints / ((*swarm)->BoidCount() - 1.0f);

		swarmCentreDir = (targetPos - currPos) * swarmCentreStrength;
	}

	//Attempt to match velocity with other boids
	Vector3 matchVelocity = (*swarm)->SumOfVelocities();
	{
		const float matchVelocityStrength = 0.05f;

		matchVelocity -= physicsObject->GetLinearVelocity();
		matchVelocity /= ((*swarm)->BoidCount() - 1.0f);
		matchVelocity = (matchVelocity - physicsObject->GetLinearVelocity()) * matchVelocityStrength;
	}

	//Avoid other nearby boids as well as targets swarm has been assigned to avoid.
	Vector3 avoidDir;
	{
		const float avoidBoidDistance = 5.0f;
		const float avoidTargetsDistanceSqrd = pow(12.0f,2.0f);
		const float avoidStrength = 2.5f;
		auto nearbyBoids = game->GetWorld()->ObjectsWithinRadius(currPos, avoidBoidDistance, "boid");

		for (auto boid : nearbyBoids)
		{
			if (boid == this)
				continue;

			Vector3 displacement = boid->GetTransform().GetPosition() - currPos;

			//Falls of exponentially with distance.
			float strength = 1 / displacement.LengthSquared();
			avoidDir -= displacement * strength;
		}

		for (auto target : (*swarm)->avoidTargets) {
			Vector3 displacement = target->GetTransform().GetPosition() - currPos;

			if (displacement.LengthSquared() < avoidTargetsDistanceSqrd) {
				avoidDir -= displacement * avoidStrength;
			}
		}
	}

	//Tend slightly towards the center. This is to avoid boids getting clumped in edges and corners.
	const float worldCentreStrength = 0.003f;
	Vector3 centreDir = (Vector3() - currPos) * worldCentreStrength;

	//Combine our calculated forces and normalise to get a single direction vector
	Vector3 finalDir = swarmCentreDir + avoidDir + matchVelocity + centreDir;
	finalDir.y = 0.0f;
	finalDir.Normalise();

	physicsObject->AddForce(finalDir * speed);
}

void BoidObject::OnCollisionBegin(GameObject* otherObject) {
	
	if (otherObject->HasTag("actor")) {
		auto actor = (ActorObject*)otherObject;

		actor->AddScore(1);
		actor->AddSpeed(0.02f);
		Transform& transform = actor->GetTransform();
		transform.SetScale(transform.GetScale() + Vector3(0.1f, 0.1f, 0.1f));

		CapsuleVolume* volume = (CapsuleVolume*)actor->GetBoundingVolume();
		volume->SetHalfHeight(volume->GetHalfHeight() + 0.08f);
		volume->SetRadius(volume->GetRadius() + 0.03f);
	
		isActive = false;
	}
}