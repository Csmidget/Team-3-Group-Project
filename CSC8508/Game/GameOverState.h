#pragma once

#include "../Engine/PushdownState.h"
namespace NCL {
	namespace CSC8508 {

		class Game;

		class GameOverState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			void OnAwake() override;
		public:
			void SetScore(int score) { gameScore = score; };

		protected:
			Game* g2 = nullptr;
			int gameScore = 0;
		};

	}
}
