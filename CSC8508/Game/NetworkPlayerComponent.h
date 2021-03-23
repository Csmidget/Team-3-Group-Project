#pragma once
#include "../Engine/Component.h"
#include "../../Common/Quaternion.h"




namespace NCL {
	namespace CSC8508 {
		class NetworkPlayerComponent : public Component
		{

			public:
				NetworkPlayerComponent(GameObject* object, int id);
				void Update(float dt);
				void SetOrientation(Maths::Quaternion orientation) {
					this->orientation = orientation;
				};
				void SetScore(int score) { this->score = score; };
				int GetScore() const { return score; }
				void Finish();
				bool isFinished() { return isLevelFinished; }
		

			private:
				Maths::Quaternion orientation;
				int score;
				bool isLevelFinished = false;
				int playerID = -1;
		};

	}
}