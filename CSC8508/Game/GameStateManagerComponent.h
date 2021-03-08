#pragma once
#include "../Engine/Component.h"
#include <vector>
#include "../Engine/GameObject.h"
#include "../Engine/GameWorld.h"
#include "ScoreComponent.h";
namespace NCL {

	namespace CSC8508 {
		class GameStateManagerComponent : public Component
		{
			public:
				GameStateManagerComponent(GameObject* object, GameWorld* gameWorld);
				void Start();
				void Update(float dt);
			private:
				void RefreshPlayerList();
				bool IsGameOver();

				GameWorld* world;
				std::vector<GameObject*> players;
				int clientPlayerID = 0;
		};
	}
}
