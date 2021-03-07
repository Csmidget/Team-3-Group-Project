#pragma once
#include "D:\Github\Team-3-Group-Project\CSC8508\Engine\Component.h"
namespace NCL {

	namespace CSC8508 {
		class ScoreComponent : public Component
		{
			public:
				ScoreComponent(GameObject* object);
				int GetScore() const { return score; };
				void OnCollisionBegin(GameObject* otherObject);

			private:
				float score;
		};

	}
}