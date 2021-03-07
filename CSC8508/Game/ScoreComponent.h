#pragma once
#include "../Engine/Component.h"
namespace NCL {

	namespace CSC8508 {
		class ScoreComponent : public Component
		{
			public:
				ScoreComponent(GameObject* object);
				void OnCollisionBegin(GameObject* otherObject);
				
				int GetScore() const { return score; };
				bool IsFinished() const { return hasFinished; }
			private:
				float score;
				bool hasFinished = false;
		};

	}
}