#pragma once

#include "../Engine/GameObject.h"

namespace NCL {
	namespace CSC8508 {

		class RespawningObject : public GameObject {
			
			friend class ObjectFactory;

		protected:
			RespawningObject(Vector3 spawnPosition, string name, bool randomizeOrientation = false);

		public:
			virtual void Respawn();
			virtual void OnRespawn() {};
			virtual void OnUpdate(float dt) override;
			virtual void ObjectSpecificDebugInfo(int& currLine, float lineSpacing) const override;

			void SetRespawnPosition(Vector3 val) { spawnPosition = val; }

		private:
			void OnKill() override;
			Vector3 spawnPosition;
			bool randomizeOrientation;

			Vector3 prevPos;
			float respawnTimer;
		};
	}
}