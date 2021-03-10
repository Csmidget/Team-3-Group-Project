#pragma once
#include "../Engine/Component.h"

#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class TeleporterComponent : public Component {

		public:
			TeleporterComponent(GameObject* object, Maths::Vector3 targetPos);
			void OnCollisionBegin(GameObject* other);

		private:
			Maths::Vector3 targetPosition;
		};

	}
}