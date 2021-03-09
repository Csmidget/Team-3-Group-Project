#pragma once
#include "../Engine/Component.h"
namespace NCL {
	namespace CSC8508 {

		class BonusComponent :public Component
		{
			public:
				BonusComponent(GameObject* object, int bonus);
				
				int GetBonus() const { return bonus; }

			private:
				int bonus;
		};

	}
}