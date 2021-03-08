#pragma once
#include "btBulletDynamicsCommon.h"

#include "../../Common/Vector3.h"
#include "../../Common/Quaternion.h"
#include "../../CSC8508/Engine/Transform.h"

namespace NCL 
{
	namespace CSC8508 
	{
		namespace physics
		{
			class BulletWorld;
			class RigidBody
			{
			public:
				RigidBody(Transform* parentTransform);
				~RigidBody();

				void addBoxShape(NCL::Maths::Vector3 halfExtents);
				void addSphereShape( float radius);
				void addCapsuleShape(float radius, float height);
				void addCylinderShape(NCL::Maths::Vector3 halfExtents);
				void addConeShape(float radius, float height);

				void createBody(float mass,
								float restitution,
								float friction,
								BulletWorld* physicsWorld);
				void setUserPointer(void* object);


				btRigidBody* returnBody() { return body; };

				void setTransform();

				void updateTransform();
				
				void addForce(NCL::Maths::Vector3 force);
				void addForceAtPos(NCL::Maths::Vector3 force, NCL::Maths::Vector3 pos);
				void setLinearVelocity(NCL::Maths::Vector3 vel);
				void addTorque(NCL::Maths::Vector3 torque);
				void addImpulse(NCL::Maths::Vector3 force);
				void addTorqueImpulse(NCL::Maths::Vector3 force);

				void setDamping(float linear, float angular);

			private:

				float linearDamping = 0.05f;
				float angularDamping = 0.7f;

				//PhysicsObject* parent;
				Transform* transform;
				btRigidBody* body;
				btCollisionShape* colShape;

				BulletWorld* worldRef;

			};
		}
	}
}