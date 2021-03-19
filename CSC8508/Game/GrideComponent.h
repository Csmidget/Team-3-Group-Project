#pragma once
#include "../Engine/Component.h"

#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class GrideComponent : public Component {

		public:
			GrideComponent(GameObject* object);
			void OnCollisionStay(GameObject* other);

		private:

		};

	}
}
