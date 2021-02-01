#pragma once
#include "GameState.h"

namespace NCL {

	class Camera;

	namespace CSC8508 {

		class NavmeshTestState : public GameState {

		public:
			NavmeshTestState(Game* game);

		protected:

			int requiredBoids;
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

			Camera* camera;
		};

	}
}

