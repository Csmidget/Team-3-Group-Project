#pragma once
#include "../Engine/PushdownState.h" 

namespace NCL {
	namespace CSC8508 {
		class Game;

		class GameState : public PushdownState {

		public:
			GameState(Game* game) { this->game = game; }

		protected:
			Game* game;
		};
	}
}