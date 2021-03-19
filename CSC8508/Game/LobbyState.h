#pragma once

#include "../Engine/PushdownState.h"

namespace NCL {
	namespace CSC8508 {

		class Game;


		class LobbyState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			void OnAwake() override;
			void OnSleep() override {};

		public:
			LobbyState(Game* game, bool client);
			~LobbyState();

		protected:
			bool gameStarted;
			Game* game;	
		};
	}
}