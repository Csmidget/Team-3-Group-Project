#pragma once

#include "../Engine/PushdownState.h"

namespace NCL {
	namespace CSC8508 {

		class Game;

		class PlayState : public PushdownState {
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
		protected:
			Game* g1;
			bool isGameFinished = false;
			int score = 0;
		};

	}
}