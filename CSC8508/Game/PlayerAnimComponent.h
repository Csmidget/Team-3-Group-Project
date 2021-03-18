#pragma once

#include "../Engine/Component.h"
#include "PlayerComponent.h"

namespace NCL {

	class MeshAnimation;

	namespace CSC8508 {

		class PlayerComponent;
		class Game;

		class PlayerAnimComponent : public Component {

		public:
			PlayerAnimComponent(GameObject* object, Game* game);
			void Start() override;
			void Update(float dt) override;

		private:
			PlayerComponent* playerComp;
			PlayerMovementState currentState;

			MeshAnimation* idleAnim;
			MeshAnimation* runAnim;
			MeshAnimation* jumpAnim;

		};

	}
}