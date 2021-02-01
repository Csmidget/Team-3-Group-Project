#pragma once
#include "../Engine/BehaviourParallel.h"

namespace NCL {
	namespace CSC8508 {

		class Game;
		class AIObject;

		class KatamariAIBehaviourTree : public BehaviourParallel {

		public:
			KatamariAIBehaviourTree(Game* game, AIObject* actor);

		protected:
			AIObject* actor;
			Game* game;

		};
	}
}

