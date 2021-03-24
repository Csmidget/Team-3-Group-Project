#pragma once
#include "../Engine/PushdownState.h"
#include<string>
const int LEVELCOUNT = 3;

namespace NCL {
	namespace CSC8508 {

		class Game;
		class GameStateManagerComponent;

		class PlayState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;

		public:
			PlayState(Game* game, bool isNetworked = false);

		protected:
			void InitSpawns();

			Game* game;
			bool isNetworked;
			int levelID;
			std::string* levels;
			GameStateManagerComponent* gameStateManager;
		};

	}
}