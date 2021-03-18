#pragma once
#include "../Engine/Component.h"
#include "../Engine/PhysicsSystem.h"
namespace NCL {
	namespace CSC8508 {

		class TimeScoreComponent :public Component
		{
		public:
			TimeScoreComponent(GameObject* object);
			GameTimer getTime;
			int nowTime;
			int nextTime;
			int num = 1;
			void UpdateScore(float dt);

		private:
			float timeScore;
		};

	}
}
