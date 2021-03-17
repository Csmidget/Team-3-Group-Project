#pragma once
#include"SetListener.h"
#include"../Engine/GameObject.h"
#include"../Engine/PhysicsObject.h"


using namespace NCL;
using namespace CSC8508;

SetListener::SetListener(GameObject* object, int listenerID)
	:Component("SetListener", object), object(object)
{
	listener = new Audio::SoundListener(listenerID);
	Audio::SoundManager::AddListener();
	physicsObject = object->GetPhysicsObject();
}

SetListener::~SetListener()
{
	Audio::SoundManager::RemoveListener();
	delete listener;
}

void SetListener::Update(float dt)
{
	auto pos = object->GetTransform().GetPosition();
	auto vel = physicsObject->GetLinearVelocity();
	auto yaw = object->GetTransform().GetOrientation().ToEuler().y;
	auto forwardVec = Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, 1);
	listener->UpdateAttributes(pos, vel, forwardVec);
	Audio::SoundManager::UpdateListener(listener);
}
