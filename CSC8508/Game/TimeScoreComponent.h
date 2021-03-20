#pragma once
#include "../Engine/Component.h"

namespace NCL {
	namespace CSC8508 {

		class TimeScoreComponent : public Component
		{
			public:
				TimeScoreComponent(GameObject* object);

				void Update(float dt) override;
				int GetScore() const;

			protected:
				float timeScore;
		};

	}
}
