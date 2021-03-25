#pragma once

#include "../Engine/Component.h"
#include"../../Common/Vector4.h"

namespace NCL {

	namespace CSC8508 {

		class Game;

		class ScoreComponent : public Component
		{
			public:

				static ScoreComponent* instance; 
				static void DisplayScoreboard(Game* game);

				ScoreComponent(GameObject* object);
				~ScoreComponent();
				
				int GetScore() const { return score; }

				void AddScore(int val);

				bool IsFinished() const { return hasFinished; }
			private:
				int score;
				bool hasFinished = false;
		};
	}
}