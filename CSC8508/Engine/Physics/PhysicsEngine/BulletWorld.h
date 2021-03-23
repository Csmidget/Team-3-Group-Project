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
			//converts a vector3 to a btVector
			inline btVector3 convertVector3(NCL::Maths::Vector3 vector)
			{
				return btVector3(vector.x, vector.y, vector.z);
			}
			//converts a quaternion to a bt quaternion
			inline btQuaternion convertQuaternion(NCL::Maths::Quaternion original)
			{
				return btQuaternion(original.x, original.y, original.z, original.w);
			}
			//converts a btVector3 to a Vector3
			inline NCL::Maths::Vector3 convertbtVector3(btVector3 original)
			{
				return NCL::Maths::Vector3(original.x(), original.y(), original.z());
			}

			typedef std::pair<const btCollisionObject*, const btCollisionObject*> collisionPair;

			class RigidBody;

			class BulletWorld
			{
			public:
				BulletWorld();
				~BulletWorld();

				void setGravity(NCL::Maths::Vector3 force);
				GameObject* rayIntersect(	NCL::Maths::Vector3 from, NCL::Maths::Vector3 to,
									/*OUT*/ NCL::Maths::Vector3 pointHit);

				void addRigidBody(RigidBody* body);
				void addpointconstraint(RigidBody* bodyA, NCL::Maths::Vector3 point);
				void addhingeconstraint(RigidBody* doorbody, NCL::Maths::Vector3 point, NCL::Maths::Vector3 axisA);
				
				void removeRigidBody(RigidBody* body);

				void Update(float dt);
				void checkCollisions();
				void clear();

			private:
				static void tickCallBack(btDynamicsWorld* world, btScalar timeStep);
				void updateObjects(float dt);

				btDefaultCollisionConfiguration* collisionConfiguration;
				btCollisionDispatcher* dispatcher;
				btBroadphaseInterface* overlappingPairCache;
				btSequentialImpulseConstraintSolver* solver;
			
				btDiscreteDynamicsWorld* dynamicsWorld;

				std::vector<RigidBody*> rigidList;
				std::vector<collisionPair> contactList;
				std::vector<btTypedConstraint*> constraintList;
			};
		}
	}
}

