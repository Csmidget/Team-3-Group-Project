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
	destroy			= false;
	persistent		= false;
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

void GameObject::SetIsActive(bool val) {

	if (isActive == val)
		return;

	isActive = val;

	if (physicsObject && physicsObject->body)
		physicsObject->body->setActive(val);

	if (val == true) {
		for (auto component : components) {
			if (component->IsEnabled())
				component->OnActive();
		}
	}
}

void GameObject::Start() {

	for (int i{ 0 }; i < components.size(); ++i)
	{
		components[i]->Start();
	}
}

void GameObject::Update(float dt)
{
	if (!isActive)
		return;

	for (auto component : components) {
		if (component->IsEnabled())
			component->Update(dt);
	}

	if(renderObject)
		renderObject->Update(dt);

	OnUpdate(dt);
}

void GameObject::fixedUpdate(float dt)
{
	if (!isActive)
		return;

	for (auto component : components) {
		if (component->IsEnabled())
			component->fixedUpdate(dt);
	}
}

void GameObject::OnCollisionBegin(GameObject* otherObject)
{
	if (!isActive)
		return;

	for (auto component : components) {
		component->OnCollisionBegin(otherObject);
	}
}

void GameObject::OnCollisionStay(GameObject* otherObject)
{
	if (!isActive)
		return;

	for (auto component : components) {
		component->OnCollisionStay(otherObject);
	}
}

void GameObject::OnCollisionEnd(GameObject* otherObject)
{
	if (!isActive)
		return;

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

	info.push_back("Name: " + name);
	
	auto transformInfo = transform.GetDebugInfo();
	info.insert(info.end(), transformInfo.begin(), transformInfo.end());

	auto physicsInfo = physicsObject->body->debugInfo();
	info.insert(info.end(), physicsInfo.begin(), physicsInfo.end());

	for (auto comp : components) {
		auto compInfo = comp->GetDebugInfo();
		info.insert(info.end(), compInfo.begin(), compInfo.end());
	}

	return info;
}

void GameObject::UpdateBroadphaseAABB() {
	if (!isActive)
		return;

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