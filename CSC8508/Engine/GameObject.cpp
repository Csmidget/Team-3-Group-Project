#include "GameObject.h"
#include "GameWorld.h"
#include "CollisionDetection.h"
#include "Debug.h"
#include "Component.h"
#include <sstream>

using namespace NCL::CSC8508;

GameObject::GameObject(string objectName) : transform(this)	{
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

	for (auto component : components)
		delete component;
	
	components.clear();

}

void GameObject::Start() {
	for (auto component : components) {
		component->Start();
	}
}

void GameObject::Update(float dt)
{
	for (auto component : components) {
		if (component->IsEnabled())
			component->Update(dt);
	}

	OnUpdate(dt);
}

void GameObject::OnCollisionBegin(GameObject* otherObject)
{
	for (auto component : components) {
		component->OnCollisionBegin(otherObject);
	}
}

void GameObject::OnCollisionStay(GameObject* otherObject)
{
	for (auto component : components) {
		component->OnCollisionStay(otherObject);
	}
}

void GameObject::OnCollisionEnd(GameObject* otherObject)
{
	for (auto component : components) {
		component->OnCollisionEnd(otherObject);
	}
}

void GameObject::OnKill() {
	isActive = false;
	for (auto component : components) {
		component->OnKill();
	}
}

bool GameObject::GetBroadphaseAABB(Vector3&outSize) const {
	if (!boundingVolume) {
		return false;
	}
	outSize = broadphaseAABB;
	return true;
}

std::vector<std::string> GameObject::DebugInfo() const {
	std::vector<std::string> info;
	
	for (auto comp : components) {
		auto compInfo = comp->GetDebugInfo();
		info.insert(info.end(), compInfo.begin(), compInfo.end());
	}

	return info;
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

void GameObject::SetGameWorld(GameWorld* newWorld) {
	if (world != nullptr)
		throw std::exception("Attempted to assign gameObject to multiple worlds!");

	world = newWorld;
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