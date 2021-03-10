#pragma once
#include "../Engine/Component.h"

#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {

		class RespawnComponent : public Component 
		{
		public:
			RespawnComponent(GameObject* object);
			void Start() override;
			void OnKill() override;

		private:
			Maths::Vector3 spawnPosition;
		};

	}
}