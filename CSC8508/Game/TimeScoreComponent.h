#pragma once
#include "../Engine/Component.h"

namespace NCL {
	namespace CSC8508 {

		class ScoreComponent;
		class Game;

		class TimeScoreComponent : public Component
		{
			public:
				TimeScoreComponent(GameObject* object, Game* game, int strength);

				void Update(float dt) override;

			protected:
				float timer;
				int strength;

		};

	}
}
