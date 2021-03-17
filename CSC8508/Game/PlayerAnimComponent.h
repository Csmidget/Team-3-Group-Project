#pragma once

#include "../Engine/Component.h"

namespace NCL {
	namespace CSC8508 {

		class PlayerComponent;
		class Game;

		class PlayerAnimComponent : public Component {

		public:
			PlayerAnimComponent(GameObject* object) : Component("PlayerAnim", object) {};
			void Start() override;
			void Update(float dt) override;

		private:
			PlayerComponent* playerComp;
		};

	}
}