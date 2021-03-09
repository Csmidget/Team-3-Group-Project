#include "RigidBody.h"
#include "BulletWorld.h"

using namespace NCL;
using namespace CSC8508;
using namespace physics;



RigidBody::RigidBody(Transform* parentTransform)
{
	body = nullptr;
	colShape = nullptr;
	worldRef = nullptr;
	transform = parentTransform;
}

RigidBody::~RigidBody()
{
	if (worldRef)
		worldRef->removeRigidBody(this);

	if (body)
	{
		delete body->getMotionState();
		delete body;
	}

	if (colShape)
		delete colShape;
}

// collisoin shapes based on several primitives. Must be called before creating a body

void RigidBody::addBoxShape(NCL::Maths::Vector3 halfExtents)
{
	colShape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
}

void RigidBody::addSphereShape(float radius)
{
	colShape = new btSphereShape(radius);
}

void RigidBody::addCapsuleShape(float radius, float height)
{
	colShape = new btCapsuleShape(radius, height);
}

void RigidBody::addCylinderShape(NCL::Maths::Vector3 halfExtents)
{
	colShape = new btCylinderShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
}

void RigidBody::addConeShape(float radius, float height)
{
	colShape = new btConeShape(radius, height);
}


void RigidBody::addForce(NCL::Maths::Vector3 force)
{
	if (body)
	{
		btVector3 pushForce = convertVector3(force);
		body->applyCentralForce(pushForce);
		body->activate();
	}
}

void RigidBody::setLinearVelocity(NCL::Maths::Vector3 vel)
{
	if (body)
	{
		btVector3 velocity = convertVector3(vel);
		body->setLinearVelocity(velocity);
		body->activate();
	}
}

void RigidBody::addForceAtPos(NCL::Maths::Vector3 force, NCL::Maths::Vector3 pos)
{
	if (body)
	{
		btVector3 pushForce = convertVector3(force);
		btVector3 relPos = convertVector3(pos);
		body->applyForce(pushForce, relPos);
		body->activate();
	}
}

void RigidBody::addTorque(NCL::Maths::Vector3 torque)
{
	if (body)
	{
		btVector3 addedTorque = convertVector3(torque);
		body->applyTorque(addedTorque);
		body->activate();
	}
}

void RigidBody::addImpulse(NCL::Maths::Vector3 force)
{
	if (body)
	{
		btVector3 pushForce = convertVector3(force);
		body->applyCentralPushImpulse(pushForce);
		body->activate();
	}
}

void RigidBody::addTorqueImpulse(NCL::Maths::Vector3 torque)
{
	if (body)
	{
		btVector3 addedTorque = convertVector3(torque);
		body->applyTorqueTurnImpulse(addedTorque);
		body->activate();
	}
}

void RigidBody::updateTransform()
{
	if (body)
	{
		btMotionState* shapeMotionTransform;
		shapeMotionTransform = body->getMotionState();
		btTransform worldTransform;
		shapeMotionTransform->getWorldTransform(worldTransform);

		btVector3 pos = worldTransform.getOrigin();
		btQuaternion rotation = worldTransform.getRotation();

		Vector3 returnPos = Vector3(pos.x(), pos.y(), pos.z());
		Quaternion returnRotation = Quaternion(rotation.x(), rotation.y(), rotation.z(), rotation.w());
		transform->SetPosition(returnPos, false);
		transform->SetOrientation(returnRotation, false);
	}
}

void RigidBody::setTransform()
{
	if (body)
	{
		NCL::Maths::Vector3 eulerAngles = transform->GetOrientation().ToEuler();

		btQuaternion rotation;
		rotation.setEulerZYX(eulerAngles.z, eulerAngles.y, eulerAngles.x);

		NCL::Maths::Vector3 SetPosition = transform->GetPosition();
		btVector3 position = btVector3(SetPosition.x, SetPosition.y, SetPosition.z);

		body->setCenterOfMassTransform(btTransform(rotation, position));
	}
}

void RigidBody::createBody(	float mass,
							float restitution,
							float friction,
							BulletWorld* physicsWorld)
{
	if (colShape)
	{
		worldRef = physicsWorld;

		btQuaternion rotation = convertQuaternion(transform->GetOrientation());
		btVector3 position = convertVector3(transform->GetPosition());

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, position));
		btScalar bodyMass = mass;
		btVector3 bodyInertia;
		colShape->calculateLocalInertia(bodyMass, bodyInertia);

		btRigidBody::btRigidBodyConstructionInfo bodyInfo = btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, colShape, bodyInertia);

		bodyInfo.m_restitution = restitution;
		bodyInfo.m_friction = friction;
	
		body = new btRigidBody(bodyInfo);

		body->setDamping(linearDamping, angularDamping);
		worldRef->addRigidBody(this);
	}
	
}

void RigidBody::setUserPointer(void* object)
{
	if(body)
		body->setUserPointer(object);
}

void RigidBody::setDamping(float linear, float angular)
{
	if (body)
	{
		linearDamping = linear;
		angularDamping = angular;

		body->setDamping(linearDamping, angularDamping);
	}
	
}


NCL::Maths::Vector3 RigidBody::getForce()
{
	if(body)
		return convertbtVector3(body->getTotalForce());
}

void RigidBody::clearForces()
{
	if(body)
		body->clearForces();
}

NCL::Maths::Vector3 RigidBody::getLinearVelocity()
{
	if (body)
		return convertbtVector3(body->getLinearVelocity());
}

void RigidBody::makeTrigger()
{
	if (body)
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		if (body->getMass() > 0)
		{
			btScalar bodyMass = 0;
			btVector3 bodyInertia;
			colShape->calculateLocalInertia(bodyMass, bodyInertia);
			body->setMassProps(0,bodyInertia);
		}
	}
}

void RigidBody::makeKinematic()
{
	if (body)
	{
		isKinemtic = true;
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	}

}