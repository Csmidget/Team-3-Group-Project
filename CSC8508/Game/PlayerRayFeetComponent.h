#pragma once
#include "../Engine/Component.h"

namespace NCL {
	namespace CSC8508 {

		class Game;
		class PlayerRayFeetComponent : public Component
		{
		public:
			PlayerRayFeetComponent(GameObject* object, Game* game);

			void Update(float dt) override;

		private:
			Game* game;
		};
	}
}