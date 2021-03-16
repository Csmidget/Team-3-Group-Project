#pragma once
#include "../Engine/PushdownState.h"
#include <vector>

namespace NCL {
	namespace CSC8508 {

		class Game;
		class GameObject;
		class PlayerComponent;
		class CameraComponent;

		class DebugState : public PushdownState {

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			void OnAwake() override;
			void OnSleep() override;
		public:
			DebugState(Game* game);

		private:
			void UpdateCameraControls(float dt);
			void DisplayDebugInfo();

			bool selectionMode;
			GameObject* selectedObject;
			int debugInfoScroll;

			Game* game;
			std::vector<PlayerComponent*> playerComponents;
			CameraComponent* oldMain;
			GameObject* debugCameraObject;
			CameraComponent* debugCamera;
		};

	}
}
