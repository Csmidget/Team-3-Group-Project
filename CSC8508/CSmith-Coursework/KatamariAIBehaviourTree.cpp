#include "KatamariAIBehaviourTree.h"

#include "BonusObject.h"
#include "AIObject.h"
#include "Game.h"

#include "../Engine/GameWorld.h"
#include "../Engine/BehaviourAction.h"
#include "../Engine/BehaviourSequence.h"
#include "../Engine/BehaviourSelector.h"

using namespace NCL::CSC8508;

//An incredibly simple behaviour tree.
KatamariAIBehaviourTree::KatamariAIBehaviourTree(Game* g, AIObject* a) : BehaviourParallel("Race AI Root") {
	game = g;
	actor = a;

	BehaviourSequence* root = new BehaviourSequence("root");
	AddChild(root);

	root->AddChild(new BehaviourAction("Hunt nearest boid", [&](float dt, BehaviourState state)->BehaviourState {
		actor->SetCurrentState("Chasing Boids");
		auto boids = game->GetWorld()->ObjectsWithinRadius(actor->GetTransform().GetPosition(), 100.0f, "boid");
		if (boids.empty())
			return BehaviourState::Failure;

		for (auto boid : boids) {
			Vector3 boidPos = boid->GetTransform().GetPosition();
			if (boidPos.LengthSquared() < 2025) { //45 squared, this is just smaller than the size of the arena.
				actor->SetGoal(boidPos);
				return BehaviourState::Ongoing;
			}
		}		
		return BehaviourState::Failure;
	}));
}