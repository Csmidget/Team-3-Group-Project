#pragma once

#include "RespawningObject.h"

namespace NCL {
	namespace CSC8508 {
		
		class GameWorld;
		class ActorObject;

		class BonusObject : public RespawningObject {

			friend class ObjectFactory;

		public:
			void Update(float dt) override;

			void OnCollisionBegin(GameObject* otherObject);

			bool IsPopped() const { return popped; }

		protected:

			BonusObject(Vector3 respawnPosition, float respawnDelay = 5.0f) : RespawningObject(respawnPosition, "bonus") {
				popped = false;
				respawnTimer = respawnDelay;
				this->respawnDelay = respawnDelay;
				tags.push_back("bonus");
			}

			virtual void ApplyBonus(ActorObject* target) = 0;

		private:
			bool popped;
			float respawnDelay;
			float respawnTimer;
			Vector3 savedScale;
		};
	}
}