#pragma once
#include "../Engine/Component.h"
#include "../Engine/NetworkManager.h"
#include "ScoreComponent.h"




namespace NCL {
	namespace CSC8508 {

		class Game; 
		class GameStateManagerComponent;

		class LocalNetworkPlayerComponent :public Component
		{
			public:
				LocalNetworkPlayerComponent(GameObject* object,Game* game, LocalPlayer* localPlayer);
				void SetTransform(const Vector3& pos, const Quaternion& orientation);
				void Update(float dt);
			protected:
				LocalPlayer* localPlayer;
				GameStateManagerComponent* gameManager;
		};

	}
}