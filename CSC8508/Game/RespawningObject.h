#pragma once

#include "../Engine/GameObject.h"


namespace NCL {

	namespace CSC8508 {

		class RespawningObject : public GameObject {

		public:
			RespawningObject(Vector3 spawnPosition,bool randomizeOrientation = false, string name = "");
			~RespawningObject();
			void OnKill() override;

		protected:
			Vector3 spawnPosition;
			bool randomizeOrientation;
		};
	}
}