#include "RigidBody.h"
#include "BulletWorld.h"
#include "../../CSC8508/Engine/Debug.h"

#include <sstream>
#include <iomanip>

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
		body->applyCentralImpulse(pushForce);
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
		btQuaternion rotation = convertQuaternion(transform->GetOrientation());

		NCL::Maths::Vector3 SetPosition = transform->GetPosition();
		btVector3 position = convertVector3(SetPosition);
		
		btTransform newTransform;
		newTransform.setOrigin(position);
		newTransform.setRotation(rotation);
		body->setWorldTransform(newTransform);
		body->getMotionState()->setWorldTransform(newTransform);
	}
}

void RigidBody::setOrientation()
{
	if (body)
	{
		btQuaternion rotation = convertQuaternion(transform->GetOrientation());

		btTransform trans = body->getWorldTransform();
		trans.setRotation(rotation);
		body->setWorldTransform(trans);
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

void RigidBody::setFriction(float friction)
{
	if(body)
		body->setFriction(friction);
}

void RigidBody::setRestitution(float restitutuion)
{
	if(body)
		body->setRestitution(restitutuion);
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

std::vector<std::string> RigidBody::debugInfo()
{
	vector<std::string> returnInfo;
	returnInfo.push_back("Physics Info");

	std::stringstream stream;
	stream << std::fixed << std::setprecision(2);
	stream << "  Mass: " << body->getMass();
	returnInfo.push_back(stream.str());
	stream.str("");

	Vector3 force = convertbtVector3(body->getLinearVelocity());

	if(force.Length() > 0)
		Debug::DrawLine(transform->GetPosition(), transform->GetPosition() + force.Normalised(), NCL::Maths::Vector4(1, 0, 0, 1));

	
	stream << "  Linear Velocity: " << force.x << "," << force.y << "," << force.z;
	returnInfo.push_back(stream.str());
	stream.str("");

	stream << "  Velocity Magnitude: " << force.Length();
	returnInfo.push_back(stream.str());
	stream.str("");

	int shape = colShape->getShapeType();
	switch (shape)
	{
	case BOX_SHAPE_PROXYTYPE: 
		stream << "  Collision Shape:  Box";
		break;
	case SPHERE_SHAPE_PROXYTYPE:
		stream << "  Collision Shape:  Sphere";
		break;
	case CAPSULE_SHAPE_PROXYTYPE:
		stream << "  Collision Shape:  Capsule";
		break;
	case CONE_SHAPE_PROXYTYPE:
		stream << "  Collision Shape:  Cone";
		break;
	case CYLINDER_SHAPE_PROXYTYPE:
		stream << "  Collision Shape:  Cylinder";
		break;
	default:
		stream << "  Collision Shape:  none Primitive";
		break;
	}
	
	returnInfo.push_back(stream.str());
	stream.str("");

	return returnInfo;
}