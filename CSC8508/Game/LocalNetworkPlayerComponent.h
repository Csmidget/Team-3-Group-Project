#pragma once
#include "../Engine/Component.h"
#include "../../Common/Vector3.h"
#include "../../Common/Quaternion.h"

namespace NCL {
	namespace CSC8508 {

		class Game; 
		class GameStateManagerComponent;
		class LocalPlayer;

		class LocalNetworkPlayerComponent :public Component
		{
			public:
				LocalNetworkPlayerComponent(GameObject* object, LocalPlayer* localPlayer);
				void SetTransform(const Maths::Vector3& pos, const Maths::Quaternion& orientation);
				void SetGameFinished(bool val);
				void Update(float dt);
			protected:
				LocalPlayer* localPlayer;
		};
	}
}