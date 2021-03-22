#pragma once

#include "../Engine/Component.h"
namespace NCL {

	namespace CSC8508 {
		class ScoreComponent : public Component
		{
			public:

				static ScoreComponent* instance; 

				ScoreComponent(GameObject* object);
				~ScoreComponent();
				void OnCollisionBegin(GameObject* otherObject);
				
				int GetScore() const { return score; }

				void AddScore(int val);

				bool IsFinished() const { return hasFinished; }
			private:
				int score;
				bool hasFinished = false;
		};
	}
}