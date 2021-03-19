#pragma once
#include "../Engine/Component.h"
#include "../../Common/Quaternion.h"




namespace NCL {
	namespace CSC8508 {
		class NetworkPlayerComponent : public Component
		{

			public:
				NetworkPlayerComponent(GameObject* object);
				void Update(float dt);
				void SetOrientation(Maths::Quaternion orientation) {
					this->orientation = orientation;
				};
				void SetScore(int score) { this->score = score; };
				bool isFinished() { return isLevelFinished; }
				void Finish();


			private:
				Maths::Quaternion orientation;
				int score;
				bool isLevelFinished = false;
		};

	}
}