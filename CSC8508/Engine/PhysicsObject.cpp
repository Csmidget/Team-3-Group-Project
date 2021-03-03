#include "PhysicsObject.h"
#include "PhysicsSystem.h"
#include "Transform.h"
#include <iomanip>
#include <sstream>
#include "Debug.h"


using namespace NCL;
using namespace CSC8508;

PhysicsObject::PhysicsObject(Transform* parentTransform, const CollisionVolume* parentVolume)	{
	transform	= parentTransform;
	volume		= parentVolume;

	inverseMass = 1.0f;
	elasticity	= 0.4f;
	friction	= 0.4f;

	body = new physics::RigidBody(parentTransform);
}

PhysicsObject::~PhysicsObject()	{

}

void PhysicsObject::ApplyAngularImpulse(const Vector3& force) {

	body->addTorqueImpulse(force);
	
}

void PhysicsObject::ApplyLinearImpulse(const Vector3& force) {
	body->addImpulse(force);
}

void PhysicsObject::AddForce(const Vector3& addedForce) {
	body->addForce(addedForce);
}

void PhysicsObject::AddForceAtPosition(const Vector3& addedForce, const Vector3& position) {
	Vector3 localPos = position - transform->GetPosition();
	body->addForceAtPos(addedForce, position);
}

void PhysicsObject::AddTorque(const Vector3& addedTorque) {
	body->addTorque(addedTorque);
}


void PhysicsObject::ClearForces() {
	force				= Vector3();
	torque				= Vector3();
}

void PhysicsObject::InitCubeInertia() {
	Vector3 dimensions	= transform->GetScale();

	Vector3 fullWidth = dimensions * 2;

	Vector3 dimsSqr		= fullWidth * fullWidth;

	inverseInertia.x = (12.0f * inverseMass) / (dimsSqr.y + dimsSqr.z);
	inverseInertia.y = (12.0f * inverseMass) / (dimsSqr.x + dimsSqr.z);
	inverseInertia.z = (12.0f * inverseMass) / (dimsSqr.x + dimsSqr.y);
}

//This is actually just a stretched version of the cube intertia.
void PhysicsObject::InitCapsuleInertia(float height, float radius) {
	Vector3 dimensions = Vector3(radius, height, radius);

	Vector3 fullWidth = dimensions * 2;
	Vector3 dimsSqr = fullWidth * fullWidth;



	inverseInertia.x = (12.0f * inverseMass) / (dimsSqr.y + dimsSqr.z);
	inverseInertia.y = (12.0f * inverseMass) / (dimsSqr.x + dimsSqr.z);
	inverseInertia.z = (12.0f * inverseMass) / (dimsSqr.x + dimsSqr.y);
}

void PhysicsObject::InitSphereInertia() {
	float radius	= transform->GetScale().GetMaxElement();
	float i			= 2.5f * inverseMass / (radius*radius);

	inverseInertia	= Vector3(i, i, i);
}

void PhysicsObject::InitHollowSphereInertia() {
	float radius = transform->GetScale().GetMaxElement();
	float i = 1.5f * inverseMass / (radius * radius);

	inverseInertia = Vector3(i, i, i);
}

void PhysicsObject::UpdateInertiaTensor() {
	Quaternion q = transform->GetOrientation();
	
	Matrix3 invOrientation	= Matrix3(q.Conjugate());
	Matrix3 orientation		= Matrix3(q);

	inverseInteriaTensor = orientation * Matrix3::Scale(inverseInertia) *invOrientation;
}

void PhysicsObject::PrintDebugInfo(int& currLine, float lineSpacing) const {
	std::stringstream stream;

	stream << std::fixed << std::setprecision(2);

	stream << "LinVel: " << linearVelocity;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	Debug::DrawLine(transform->GetPosition(), transform->GetPosition() + linearVelocity,Vector4(0,0,1,1));

	stream << "AngVel: " << angularVelocity;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Friction: " << friction;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	stream << "Elasticity: " << elasticity;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

}