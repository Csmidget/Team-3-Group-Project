#pragma once

#include "../Engine/PushdownState.h"

namespace NCL {
	namespace CSC8508 {

		class Game;

		class PauseState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;

		public:
			PauseState(Game* game) {
				this->game = game;
			}

		protected:
			Game* game;
		};

	}
}
