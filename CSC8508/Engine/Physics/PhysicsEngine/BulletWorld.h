#pragma once
#include "btBulletDynamicsCommon.h"

namespace NCL
{
	namespace CSC8508
	{
		namespace physics
		{
			class BulletWorld
			{
			public:
				BulletWorld();
				~BulletWorld();

				void setGravity();
				void addRigidBody(btRigidBody* body);

				void Update(float dt);
			private:
				btDefaultCollisionConfiguration* collisionConfiguration;
				btCollisionDispatcher* dispatcher;
				btBroadphaseInterface* overlappingPairCache;
				btSequentialImpulseConstraintSolver* solver;

				btDiscreteDynamicsWorld* dynamicsWorld;
			};
		}
	}
}

