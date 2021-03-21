#pragma once
#include "../Engine/Component.h"
#include "../Engine/NetworkManager.h"
#include "ScoreComponent.h"




namespace NCL {
	namespace CSC8508 {
		class LocalNetworkPlayerComponent :public Component
		{
			public:
				LocalNetworkPlayerComponent(GameObject* object, LocalPlayer* localPlayer);
				void Update(float dt);
			protected:
				LocalPlayer* localPlayer;
				ScoreComponent* score;
		};

	}
}