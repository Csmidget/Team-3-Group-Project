#pragma once
#include "../Engine/Component.h"
#include <vector>
#include "../Engine/GameObject.h"
#include "ScoreComponent.h";
#include "TimeScoreComponent.h"
namespace NCL {

	namespace CSC8508 {

		class Game;

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
				bool IsGameOver();

				Game* game;
				bool isPlayerFinished;
				bool isGameFinished;
				bool majorityFinished;
				float finishTimer;
		};
	}
}
