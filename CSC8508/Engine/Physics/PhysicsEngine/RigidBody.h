#pragma once
#include "btBulletDynamicsCommon.h"
#include "BulletWorld.h"

#include "../../Common/Vector3.h"
#include "../../Common/Quaternion.h"
namespace NCL 
{
	namespace CSC8508 
	{
		namespace physics
		{
			class RigidBody
			{
			public:
				RigidBody();
				~RigidBody();

				void addBoxShape(NCL::Maths::Vector3 halfExtents);
				void addSphereShape( float radius);
				void addCapsuleShape(float radius, float height);
				void addCylinderShape(NCL::Maths::Vector3 halfExtents);
				void addConeShape(float radius, float height);

				void createBody(NCL::Maths::Vector3 SetPosition,
								NCL::Maths::Quaternion SetRotation,
								float mass,
								float restitution,
								float friction);

				btRigidBody* returnBody() { return body; };

			private:

				btRigidBody* body;
				btCollisionShape* colShape;

			};
		}
	}
}