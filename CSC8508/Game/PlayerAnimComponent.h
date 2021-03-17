#pragma once

#include "../Engine/Component.h"

namespace NCL {
	namespace CSC8508 {

		class PlayerAnimComponent : public Component {

		public:
			PlayerAnimComponent(GameObject* object) {};
			void Start() override;
			void OnKill() override;
		};

	}
}