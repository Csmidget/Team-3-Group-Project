#pragma once

#include "../Engine/PushdownState.h"

namespace NCL {
	namespace CSC8508 {

		class Game;
		class GameWorld;
		class GameObject;

		class IntroState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			void OnAwake() override;
			void OnSleep() override;

		public:
			IntroState(Game* game);

		protected:
			Game* game;
			GameWorld* world;

			GameObject* singleCube;
			GameObject* clientCube;
			GameObject* serverCube;
			GameObject* exitCube;
		};
	}
}