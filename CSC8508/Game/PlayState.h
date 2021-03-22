#pragma once

#include "../Engine/PushdownState.h"

namespace NCL {
	namespace CSC8508 {

		class Game;
		class GameStateManagerComponent;

		class PlayState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;

		public:
			PlayState(Game* game);

		protected:
			Game* game;

			GameStateManagerComponent* gameStateManager;
		};

	}
}