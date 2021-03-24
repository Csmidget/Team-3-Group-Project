#pragma once
#include "../Engine/Component.h"
#include "../../Common/Quaternion.h"
#include "../../Common/Vector3.h"

namespace NCL {
	namespace CSC8508 {
		class NetworkPlayerComponent : public Component
		{

			public:
				NetworkPlayerComponent(GameObject* object, int id);
				void Update(float dt);
				void SetTargetPosition(Maths::Vector3 target);
				void SetOrientation(Maths::Quaternion orientation) {
					this->orientation = orientation;
				};
				void SetScore(int score) { this->score = score; };
				int GetScore() const { return score; }
				bool isFinished() { return isLevelFinished; }
				void SetIsFinished(bool isFinished) ;
				void OnActive() override;
				int GetPlayerID() const { return playerID; }

			private:
				Maths::Vector3 targetPosition;
				Maths::Vector3 previousPosition;
				float progress;
				float timeFrame;

				Maths::Quaternion orientation;
				int score;
				bool isLevelFinished = false;
				int playerID = -1;
		};

	}
}