#pragma once
#include "../Engine/Component.h"
#include <vector>
namespace NCL {

	namespace CSC8508 {

		class Game;
		class GameObject;

		class GameStateManagerComponent : public Component
		{
			public:
				static GameStateManagerComponent* instance;

				GameStateManagerComponent(GameObject* object, Game* game);
				~GameStateManagerComponent();
				void Start();
				void Update(float dt);
				void SetPlayerFinished(bool val) { isPlayerFinished = val; }
				bool IsGameFinished() const { return isGameFinished; }
				bool IsPlayerFinished() const { return isPlayerFinished; }
			private:

				Game* game;
				bool isPlayerFinished;
				bool isGameFinished;
				bool majorityFinished;
				float finishTimer;
		};
	}
}
