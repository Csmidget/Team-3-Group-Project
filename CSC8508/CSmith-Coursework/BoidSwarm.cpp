#include "BoidSwarm.h"
#include "BoidObject.h"
#include "Game.h"

using namespace NCL::CSC8508;
using namespace NCL::Maths;

void BoidSwarm::Update() {

	for (auto boid : inactiveBoids) {
		if (boid->IsActive()) {
			activeBoids.insert(boid);
			inactiveBoids.erase(boid);
		}
	}

	for (auto boid : activeBoids) {
		if (!boid->IsActive()) {
			inactiveBoids.insert(boid);
			activeBoids.erase(boid);
		}
	}
	sumOfPoints = Vector3();
	sumOfVelocities = Vector3();

	//Calculating these once for the whole swarm saves on individual boid processing.
	for (auto boid : activeBoids) {
		sumOfPoints += boid->GetTransform().GetPosition();
		sumOfVelocities += boid->GetPhysicsObject()->GetLinearVelocity();
	}

}