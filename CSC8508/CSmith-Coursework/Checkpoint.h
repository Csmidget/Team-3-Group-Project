#pragma once

#include "../../Common/Vector3.h"
#include <vector>

namespace NCL {
	namespace CSC8508 {

		class GameObject;

		using namespace Maths;

		class Checkpoint {

		public:
			Checkpoint(Vector3 position, Vector3 halfDims, int priority) {
				this->position = position;
				this->halfDims = halfDims;
				this->priority = priority;
			}

			void AddSpawnPoint(Vector3 spawnPoint) {
				spawnPoints.push_back(spawnPoint);
			}

			Vector3 GetPosition() const {
				return position;
			}

			Vector3	GetSpawnPoint() const { 
				//If no spawnpoints have been set, assume the checkpoint position is the spawnpoint.
				if (spawnPoints.size() == 0)
					return position;

				return spawnPoints[rand() % spawnPoints.size()]; 
			}

			bool ReachedCheckpoint(const GameObject* otherObject) const;

			Vector3 GetHalfDims() const {
				return halfDims;
			}

			int GetPriority() const {
				return priority;
			}

		private:
			int priority;
			std::vector<Vector3> spawnPoints;
			Vector3 halfDims;
			Vector3 position;
		};

	}
}

