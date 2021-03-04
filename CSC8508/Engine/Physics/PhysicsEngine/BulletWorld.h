#pragma once
#include "btBulletDynamicsCommon.h"
#include "RigidBody.h"
#include <vector>
#include <map>

namespace NCL
{
	namespace CSC8508
	{
		namespace physics
		{

			typedef std::pair<const btCollisionObject*, const btCollisionObject*> collisionPair;
			class RigidBody;

			class BulletWorld
			{
			public:
				BulletWorld();
				~BulletWorld();

				//void setGravity();
				void addRigidBody(RigidBody* body);
				void removeRigidBody(RigidBody* body);


				void Update(float dt);
				void checkCollisions();

			private:
				btDefaultCollisionConfiguration* collisionConfiguration;
				btCollisionDispatcher* dispatcher;
				btBroadphaseInterface* overlappingPairCache;
				btSequentialImpulseConstraintSolver* solver;

				btDiscreteDynamicsWorld* dynamicsWorld;

				int frameNumber = 0;

				std::vector<RigidBody*> rigidList;


				//std::vector<btPersistentManifold> contactList;
				std::vector<collisionPair> contactList;

				//std::map<int,collisionPair> contactList;
			};
		}
	}
}

