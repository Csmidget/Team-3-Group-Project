#pragma once
#include "../Engine/PushdownState.h"
namespace NCL {
	namespace CSC8508 {

		class Game;
		class CameraComponent;
		class GameStateManagerComponent;

		class GameOverState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			void OnAwake() override;
			
			void UpdateCameraControls(float dt);

		public:
			GameOverState(Game* game, bool isFinal, bool isNetworked = false);

		protected:
			Game* game;
			int gameScore = 0;
			float timer;
			CameraComponent* spectatorCamera;	
			bool isFinal;
			bool isNetworked;
			bool allFinished;
			GameStateManagerComponent* gameStateManager;
		};

	}
}
