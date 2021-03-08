#pragma once

#include "../Engine/PushdownState.h"
namespace NCL {
	namespace CSC8508 {

		class Game;

		class GameOverState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			void OnAwake() override;

		public:
			GameOverState(Game* game) {
				this->game = game;
			}

		protected:
			Game* game;
			int gameScore = 0;
		};

	}
}
