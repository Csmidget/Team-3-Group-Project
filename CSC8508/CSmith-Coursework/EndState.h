#pragma once
#include "GameState.h"

#include <string>

namespace NCL {

	class Camera;

	namespace CSC8508 {

		class GameWorld;
		class GameObject;

		class EndState : public GameState {

		public:
			EndState(Game* game, std::string mainText, std::string subText) : GameState(game) {
				this->mainText = mainText;
				this->subText = subText;
			}

		protected:
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

			Camera* camera;
			const GameWorld* world;

			std::string mainText;
			std::string subText;

		};

	}
}

