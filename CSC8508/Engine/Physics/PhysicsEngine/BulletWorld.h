#pragma once
#include "btBulletDynamicsCommon.h"
#include "RigidBody.h"
#include <vector>

namespace NCL
{
	namespace CSC8508
	{
		namespace physics
		{
			class RigidBody;

			class BulletWorld
			{
			public:
				BulletWorld();
				~BulletWorld();

				//void setGravity();
				void addRigidBody(RigidBody* body);
				void addhingeconstraint(RigidBody* bodyA, RigidBody* bodyB);
				
				void Update(float dt);
			private:
				btDefaultCollisionConfiguration* collisionConfiguration;
				btCollisionDispatcher* dispatcher;
				btBroadphaseInterface* overlappingPairCache;
				btSequentialImpulseConstraintSolver* solver;

				btDiscreteDynamicsWorld* dynamicsWorld;

				std::vector<RigidBody*> rigidList;
			};
		}
	}
}

