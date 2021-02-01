#pragma once
#include "GameState.h"
#include <vector>

namespace NCL {

	class Camera;

	namespace CSC8508 {

		class PlayerObject;
		class AIObject;

		class RaceState : public GameState {

		public:
			RaceState(Game* game, int opponents);

		protected:
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void UpdateScores(float dt, PlayerObject* player, std::vector<AIObject*>& opponents);
			bool CheckForFinishes(PlayerObject* player, std::vector<AIObject*>& opponents);

			Camera* camera;
			float scoreTracker;
			bool playerFinished;
		};

	}
}

