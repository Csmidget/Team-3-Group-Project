#pragma once

#include "../Engine/GameObject.h"

#include <map>

namespace NCL {
	namespace CSC8508 {

		class Constraint;
		class GameWorld;
		class ForceObject : public GameObject {

			friend class ObjectFactory;

		public:
			~ForceObject();
			void OnCollisionBegin(GameObject* otherObject) override;
			void OnCollisionEnd(GameObject* otherObject) override;

			virtual void ObjectSpecificDebugInfo(int& currLine, float lineSpacing) const override;

		protected: 
			ForceObject(GameWorld* world, Vector3 position, Vector3 dimensions, Quaternion orientation, Vector3 direction, float strength, bool continuous = true);

			bool continuous;
			float strength;
			Vector3 direction;
			std::map<GameObject*, Constraint*> activeConstraints;
			GameWorld* world;
		};
	}
}

