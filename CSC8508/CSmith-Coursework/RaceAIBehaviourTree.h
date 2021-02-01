#pragma once
#include "../Engine/BehaviourParallel.h"

namespace NCL {
	namespace CSC8508 {

		class Game;
		class AIObject;
		class GameObject;
		class BehaviourSequence;
		class BonusObject;

		class RaceAIBehaviourTree : public BehaviourParallel {

		public:
			RaceAIBehaviourTree(Game* game, AIObject* actor);

		protected:

			BehaviourSequence* CreateBonusSequence();
			BehaviourSequence* CreateAngerSequence();

			Game* game;
			AIObject* actor;
			BonusObject* bonusTarget;
			GameObject*  angerTarget;
		};
	}
}

