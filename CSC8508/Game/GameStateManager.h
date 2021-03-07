#pragma once
#include <vector>
#include "../Engine/GameObject.h"
#include "../Engine/GameWorld.h"
#include "ScoreComponent.h";

namespace NCL {
	namespace CSC8508 {

		class GameStateManager
		{

			public:
				GameStateManager(GameWorld* gameWorld);
				void Update(float dt);

			private:
				void RefreshPlayerList();


				GameWorld* world;
				std::vector<GameObject*> players;
				int clientPlayerID = 0;




		};

	}
}