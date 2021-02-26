#include "GameObject.h"
#include "CollisionDetection.h"
#include "Debug.h"
#include <sstream>

using namespace NCL::CSC8508;

GameObject::GameObject(string objectName)	{
	name			= objectName;
	worldID			= -1;
	isActive		= true;
	isStatic		= false;
	boundingVolume	= nullptr;
	physicsObject	= nullptr;
	renderObject	= nullptr;
	collisionLayer = 0;
}

GameObject::~GameObject()	{
	delete boundingVolume;
	delete physicsObject;
	delete renderObject;
}

bool GameObject::GetBroadphaseAABB(Vector3&outSize) const {
	if (!boundingVolume) {
		return false;
	}
	outSize = broadphaseAABB;
	return true;
}

void GameObject::UpdateBroadphaseAABB() {
	if (!boundingVolume) {
		return;
	}

	if (boundingVolume->type == VolumeType::AABB) {
		broadphaseAABB = ((AABBVolume&)*boundingVolume).GetHalfDimensions();
	}
	else if (boundingVolume->type == VolumeType::Sphere) {
		float r = ((SphereVolume&)*boundingVolume).GetRadius();
		broadphaseAABB = Vector3(r, r, r);
	}
	else if (boundingVolume->type == VolumeType::OBB) {
		Matrix3 mat = Matrix3(transform.GetOrientation());
		mat = mat.Absolute();
		Vector3 halfSizes = ((OBBVolume&)*boundingVolume).GetHalfDimensions();
		broadphaseAABB = mat * halfSizes;
	}
	else if (boundingVolume->type == VolumeType::Capsule) {
		Matrix3 mat = Matrix3(transform.GetOrientation());
		mat = mat.Absolute();
		CapsuleVolume& volume = (CapsuleVolume&)*boundingVolume;
		Vector3 halfSizes = Vector3(volume.GetRadius(), volume.GetHalfHeight(),volume.GetRadius());
		broadphaseAABB = mat * halfSizes;
	}
}

void GameObject::PrintDebugInfo() const {
	int currLine = 0;
	float lineSpacing = 3;

	std::stringstream stream;

	stream << "Name: " << name;
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");
	stream << "Static: " << (isStatic ? "True":"False");
	Debug::Print(stream.str(), Vector2(1, ++currLine * lineSpacing));
	stream.str("");

	transform.PrintDebugInfo(++currLine, lineSpacing);

	if (physicsObject) {
		physicsObject->PrintDebugInfo(++currLine, lineSpacing);
	}

	if (boundingVolume) {
		boundingVolume->PrintDebugInfo(++currLine, lineSpacing);
	}

	ObjectSpecificDebugInfo(++currLine,lineSpacing);
}