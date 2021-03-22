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
			static GameStateManagerComponent* instance;

			public:
				GameStateManagerComponent(GameObject* object, Game* game);
				~GameStateManagerComponent();
				void Start();
				void Update(float dt);

				bool IsGameFinished() const { return isGameFinished; }
				void SetFinished(bool val) { isGameFinished = val; }
			private:
				bool IsGameOver();

				Game* game;
				bool isGameFinished;
		};
	}
}
