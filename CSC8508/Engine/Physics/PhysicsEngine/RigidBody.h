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

				void setActive(bool val);

				btRigidBody* returnBody() { return body; };

				void setTransform();
				void setOrientation();
				void updateTransform();

				void makeTrigger();
				void makeKinematic();
				
				void addForce(NCL::Maths::Vector3 force);
				void addForceAtPos(NCL::Maths::Vector3 force, NCL::Maths::Vector3 pos);
				void setLinearVelocity(NCL::Maths::Vector3 vel);
				void addTorque(NCL::Maths::Vector3 torque);
				void addImpulse(NCL::Maths::Vector3 force);
				void addTorqueImpulse(NCL::Maths::Vector3 force);

				void setDamping(float linear, float angular);
				void setFriction(float friction);
				void setRestitution( float restitutuion);


				NCL::Maths::Vector3 getForce();
				void clearForces();
				NCL::Maths::Vector3 getLinearVelocity();

				std::vector<std::string> debugInfo();

			private:

				btScalar linearDamping = 0.05f;
				btScalar angularDamping = 0.7f;

				bool isKinemtic = false;

				//PhysicsObject* parent;
				Transform* transform;
				btRigidBody* body;
				btCollisionShape* colShape;

				BulletWorld* worldRef;

			};
		}
	}
}