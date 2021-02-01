#pragma once
#include "GameState.h"

namespace NCL {

	class Camera;

	namespace CSC8508 {

		class BoidSwarm;

		class KatamariState : public GameState {

		public:
			KatamariState(Game* game, int boidLayers);
			~KatamariState();

		protected:

			int requiredBoids;
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

			Camera* camera;

			BoidSwarm* boidSwarm;
		};

	}
}

