#pragma once
#include "../Engine/PushdownState.h"
namespace NCL {
	namespace CSC8508 {

		class Game;

		class GameOverState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			void OnAwake() override;
			void PrintOutcome();
		public:
			GameOverState(Game* game, int score);

		protected:
			Game* game;
			int gameScore = 0;

			
		};

	}
}
