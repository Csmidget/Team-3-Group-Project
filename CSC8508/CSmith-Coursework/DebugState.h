#pragma once
#include "GameState.h"
#include "../../Common/Vector4.h"

namespace NCL {

	class Camera;

	namespace CSC8508 {

		class GameWorld;
		class GameObject;

		class DebugState : public GameState {

		public:
			DebugState(Game* game);
			~DebugState();

		protected:
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

			Camera* camera;
			const GameWorld* world;

			bool selectionMode;
			Maths::Vector4 selectionObjectColour;
			GameObject* selectionObject;
		};

	}
}

