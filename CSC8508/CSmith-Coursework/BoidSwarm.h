#pragma once

#include "../../Common/Vector3.h"
#include <set>

//Boid reference: https://cs.stanford.edu/people/eroberts/courses/soco/projects/2008-09/modeling-natural-systems/boids.html

namespace NCL {
	namespace CSC8508 {

		class GameObject;
		class BoidObject;
		class Game;

		class BoidSwarm {

			friend class BoidObject;

		public:
			BoidSwarm(Game* game) {
				this->game = game;
			}

			int BoidCount() const { return (int)activeBoids.size(); }

			Maths::Vector3 SumOfPoints() const { return sumOfPoints; }
			Maths::Vector3 SumOfVelocities() const { return sumOfVelocities; }

			void Update();

			void Avoid(GameObject* object) {
				avoidTargets.insert(object);
			}

		protected:

			void AddBoid(BoidObject* boid) {
				activeBoids.insert(boid);
			}

			void RemoveBoid(BoidObject* boid) {
				activeBoids.erase(boid);
				inactiveBoids.erase(boid);
			}

			Game* game;

			std::set<BoidObject*> activeBoids;
			std::set<BoidObject*> inactiveBoids;

			std::set<GameObject*> avoidTargets;

			//Used by individual boids to find their target position.
			Maths::Vector3 sumOfPoints;
			Maths::Vector3 sumOfVelocities;
		};

	}
}

