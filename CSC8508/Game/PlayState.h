#pragma once

#include "../Engine/PushdownState.h"
#include "GameStateManagerComponent.h"

namespace NCL {
	namespace CSC8508 {

		class Game;

		class PlayState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;

		public:
			PlayState(Game* game);

		protected:
			Game* game;
			int score = 0;
			bool isGameFinished = false;

			GameStateManagerComponent* GetGameStateManager() const;
		};

	}
}