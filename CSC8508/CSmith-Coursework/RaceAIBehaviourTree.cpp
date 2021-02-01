#include "RaceAIBehaviourTree.h"
  
#include "BonusObject.h"
#include "AIObject.h"
#include "Game.h"
#include "Checkpoint.h"

#include "../Engine/BehaviourSequence.h"
#include "../Engine/BehaviourAction.h"
#include "../Engine/BehaviourSelector.h"
#include "../Engine/GameWorld.h"

using namespace NCL::CSC8508;

RaceAIBehaviourTree::RaceAIBehaviourTree(Game* g, AIObject* a) : BehaviourParallel("Race AI Root") {
	game = g;
	actor = a;

	BehaviourSelector* root = new BehaviourSelector("root");
	AddChild(root);

	root->AddChild(CreateAngerSequence());

	root->AddChild(CreateBonusSequence());

	//If both anger and bonus sequences fail, continue towards the goal.
	root->AddChild(new BehaviourAction("Go to goal", [&](float dt, BehaviourState state)->BehaviourState {
		actor->SetCurrentState("Seeking goal");
		actor->SetGoal(game->GetGoal()->GetPosition());
		return BehaviourState::Success;
	}));
}

BehaviourSequence* RaceAIBehaviourTree::CreateBonusSequence() {

	BehaviourSequence* sequence = new BehaviourSequence("Bonus Sequence");

	//First check if there is a bonus in range (Or if we already have a target)
	BehaviourAction* locateBonus = new BehaviourAction("Locate Bonus", [&](float dt, BehaviourState state)->BehaviourState {

		//If we already have a target, and they are still available, keep going
		if (bonusTarget && !bonusTarget->IsPopped()) {
			return BehaviourState::Success;
		}

		auto bonusesInRange = game->GetWorld()->ObjectsWithinRadius(actor->GetTransform().GetPosition(), actor->GetCoinDetectRange(), "bonus");

		if (bonusesInRange.size() == 0)
			return BehaviourState::Failure;

		for (GameObject* o : bonusesInRange)
		{
			if (!((BonusObject*)o)->IsPopped()) {
				bonusTarget = (BonusObject*)o;
				break;
			}
		}

		//If the target is available, go for it.
		if (bonusTarget && !bonusTarget->IsPopped()) {
			return BehaviourState::Success;
		}
		else {
			bonusTarget = nullptr;
			return BehaviourState::Failure;
		}
		});
	sequence->AddChild(locateBonus);

	//If we have found a bonus to target, resolve moving towards the target.
	BehaviourAction* moveToBonus = new BehaviourAction("Move To Bonus", [&](float dt, BehaviourState state)->BehaviourState {

		Vector3 targetPosition = bonusTarget->GetTransform().GetPosition();
		targetPosition.y = 0.0f;

		if (!actor->SetGoal(targetPosition, actor->GetCoinMaxDistance())) {
			bonusTarget = nullptr;
			return BehaviourState::Failure;
		}

		if ((targetPosition - actor->GetTransform().GetPosition()).LengthSquared() < 1.0f) {
			return BehaviourState::Success;
		}
		else {
			actor->SetCurrentState("Seeking Coin");
			return BehaviourState::Ongoing;
		}

		});
	sequence->AddChild(moveToBonus);

	return sequence;
}

BehaviourSequence* RaceAIBehaviourTree::CreateAngerSequence() {
	BehaviourSequence* sequence = new BehaviourSequence("Anger Sequence");

	//Only do the sequence if the actor is above their anger threshold
	BehaviourAction* checkAnger = new BehaviourAction("Check Anger", [&](float dt, BehaviourState state)->BehaviourState {
		return (actor->IsAngry() ? BehaviourState::Success : BehaviourState::Failure);
	});
	sequence->AddChild(checkAnger);

	//If there is a target for their anger within range, path towards them.
	BehaviourAction* findTargetForAnger = new BehaviourAction("Find Anger Target", [&](float dt, BehaviourState state)->BehaviourState {

		Vector3 pos = actor->GetTransform().GetPosition();

		auto actorsInRange = game->GetWorld()->ObjectsWithinRadius(pos, 15.0f, "actor");

		if (actorsInRange.empty()) {
			return BehaviourState::Failure;
		}

		GameWorld* world = game->GetWorld();
		for (GameObject* o : actorsInRange)
		{
			//We are also an actor. So ignore ourselves.
			if (actor == o)
				continue;

			Vector3 targetPos = o->GetTransform().GetPosition();
			
			//Check whether we can SEE the target. If not, ignore.
			RayCollision col;
			Ray r = Ray(pos, targetPos - pos);
			if (world->Raycast(r, col, true, true) && (GameObject*)col.node != (GameObject*)o) {
				continue;
			}

			//Only go for a target it the path to them isn't too long.
			if (actor->SetGoal(targetPos,17.5f)) {
				actor->SetCurrentState("Fighting");
				return BehaviourState::Ongoing;
			}
		}

		return BehaviourState::Failure;
	});
	sequence->AddChild(findTargetForAnger);

	return sequence;
}