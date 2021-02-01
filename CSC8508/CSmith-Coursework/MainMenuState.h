#pragma once
#include "GameState.h"

namespace NCL {
	namespace CSC8508 {

		class GameWorld;

		class MainMenuState : public GameState {

		public:
			MainMenuState(Game* game);

		protected:
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			int selection;
			int raceOpponents;
			int katamariBoids;
			float cameraYaw;
			const GameWorld* world;
		};

	}
}

