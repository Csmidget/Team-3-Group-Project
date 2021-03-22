#pragma once
#include "../Engine/PushdownState.h"
namespace NCL {
	namespace CSC8508 {

		class Game;
		class CameraComponent;

		class GameOverState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			void OnAwake() override;
			void PrintOutcome();
			
			void UpdateCameraControls(float dt);

		public:
			GameOverState(Game* game);

		protected:
			Game* game;
			int gameScore = 0;
			CameraComponent* spectatorCamera;			
		};

	}
}
