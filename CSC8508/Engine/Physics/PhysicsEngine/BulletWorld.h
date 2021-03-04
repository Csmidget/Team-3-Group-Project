#pragma once
#include "btBulletDynamicsCommon.h"

#include "RigidBody.h"
#include "../../Common/Vector3.h"
#include "../../Common/Quaternion.h"
#include "../../CSC8508/Engine/Transform.h"
#include "../../CSC8508/Engine/GameObject.h"

#include <vector>
#include <map>

namespace NCL
{
	namespace CSC8508
	{
		namespace physics
		{
			inline btVector3 convertVector3(NCL::Maths::Vector3 vector)
			{
				return btVector3(vector.x, vector.y, vector.z);
			}

			inline btQuaternion convertQuaternion(NCL::Maths::Quaternion original)
			{
				return btQuaternion(original.x, original.y, original.z, original.w);
			}

			typedef std::pair<const btCollisionObject*, const btCollisionObject*> collisionPair;

			class RigidBody;
			//class GameObject;

			class BulletWorld
			{
			public:
				BulletWorld();
				~BulletWorld();

				void setGravity(NCL::Maths::Vector3 force);
				GameObject* rayIntersect(NCL::Maths::Vector3 from, NCL::Maths::Vector3 to);

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

				std::vector<RigidBody*> rigidList;
				std::vector<collisionPair> contactList;
			};
		}
	}
}

