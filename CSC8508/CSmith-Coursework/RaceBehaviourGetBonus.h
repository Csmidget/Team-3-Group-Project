#pragma once
#include "../CSC8503Common/BehaviourSequence.h"

namespace NCL {
	namespace CSC8503 {

		class Game;
		class AIObject;


		class RaceBehaviourGetBonus : public BehaviourSequence {

		public:
			RaceBehaviourGetBonus(Game* game, AIObject* actor);

		protected:
			Game* game;
			AIObject* actor;
		};
	}
}
