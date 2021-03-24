#pragma once

#include "../Engine/Component.h"
#include"../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class Game;

		class HingeComponent : public Component {
		public:
			HingeComponent(GameObject* object, Game* game, Maths::Vector3 point, Maths::Vector3 axis);
			~HingeComponent() {}
		};


	}
}
