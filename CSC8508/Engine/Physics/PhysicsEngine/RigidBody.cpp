#include "RigidBody.h"
#include "BulletWorld.h"

using namespace NCL;
using namespace CSC8508;
using namespace physics;

RigidBody::RigidBody(Transform* parentTransform)
{
	body = nullptr;
	colShape = nullptr;
	transform = parentTransform;
}

RigidBody::~RigidBody()
{
	if (body)
	{
		delete body->getMotionState();
		delete body;
	}

	delete colShape;
}


void RigidBody::addBoxShape(NCL::Maths::Vector3& halfExtents)
{
	colShape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
}

void RigidBody::addSphereShape(float& radius)
{
	colShape = new btSphereShape(radius);
}

void RigidBody::addCapsuleShape(float& radius, float& height)
{
	colShape = new btCapsuleShape(radius, height);
}

void RigidBody::addCylinderShape(NCL::Maths::Vector3& halfExtents)
{
	colShape = new btCylinderShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
}

void RigidBody::addConeShape(float& radius, float& height)
{
	colShape = new btConeShape(radius, height);
}


void RigidBody::addForce(NCL::Maths::Vector3 force)
{
	btVector3 pushForce;
	pushForce.setValue(force.x, force.y, force.z);
	body->applyCentralForce(pushForce);
	body->activate();
}

void RigidBody::setLinearVelocity(NCL::Maths::Vector3 vel)
{
	btVector3 velocity;
	velocity.setValue(vel.x, vel.y, vel.z);
	body->setLinearVelocity(velocity);
	body->activate();
}

void RigidBody::updateTransform()
{
	btVector3 pos = body->getCenterOfMassTransform().getOrigin();
	btQuaternion rotation = body->getCenterOfMassTransform().getRotation();

	Vector3 returnPos = Vector3(pos.x(), pos.y(), pos.z());
	Quaternion returnRotation = Quaternion(rotation.x(), rotation.y(), rotation.z(), rotation.w());
	transform->SetPosition(returnPos);
	transform->SetOrientation(returnRotation);
}

void RigidBody::createBody(	NCL::Maths::Vector3 SetPosition,
							NCL::Maths::Quaternion SetRotation,
							float mass,
							float restitution,
							float friction,
							BulletWorld* physicsWorld)
{
	NCL::Maths::Vector3 eulerAngles = SetRotation.ToEuler();

	btQuaternion rotation;
	rotation.setEulerZYX(eulerAngles.z, eulerAngles.y, eulerAngles.x);

	btVector3 position = btVector3(SetPosition.x, SetPosition.y, SetPosition.z);

	btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(rotation, position));
	btScalar bodyMass = mass;
	btVector3 bodyInertia;
	colShape->calculateLocalInertia(bodyMass, bodyInertia);

	btRigidBody::btRigidBodyConstructionInfo bodyInfo = btRigidBody::btRigidBodyConstructionInfo(bodyMass, motionState, colShape, bodyInertia);

	bodyInfo.m_restitution = restitution;
	bodyInfo.m_friction = friction;
	

	body = new btRigidBody(bodyInfo);
	body->setUserPointer(this);

	body->setDamping(0.1, 0.85);
	physicsWorld->addRigidBody(this);
	
}