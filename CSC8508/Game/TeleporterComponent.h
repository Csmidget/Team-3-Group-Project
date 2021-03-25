#pragma once
#include "../Engine/Component.h"

#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class TeleporterComponent : public Component {

		public:
			TeleporterComponent(GameObject* object, Maths::Vector3 targetPos);
			void OnCollisionStay(GameObject* other);

		private:
			std::vector<std::string> DebugInfo() override;

			Maths::Vector3 targetPosition;
		};

	}
}