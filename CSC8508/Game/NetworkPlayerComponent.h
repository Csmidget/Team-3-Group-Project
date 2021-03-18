#pragma once
#include "../Engine/Component.h"
#include "../../Common/Quaternion.h"




namespace NCL {
	namespace CSC8508 {
		class NetworkPlayerComponent : public Component
		{

			public:
				NetworkPlayerComponent(GameObject* object);
				void Update(float dt);
				void SetOrientation(Maths::Quaternion orientation) {
					this->orientation = orientation;
				};

			private:
				Maths::Quaternion orientation;
		};

	}
}