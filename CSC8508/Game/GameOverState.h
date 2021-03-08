#pragma once

#include "../Engine/PushdownState.h"
namespace NCL {
	namespace CSC8508 {

		class Game;

		class GameOverState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			void OnAwake() override;

		protected:
			Game* g2;
			int gameScore = 0;
		};

	}
}
