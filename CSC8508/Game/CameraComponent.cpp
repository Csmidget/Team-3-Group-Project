#include "CameraComponent.h"
#include "../Engine/Transform.h"
#include "../../Common/Camera.h"
#include "../../Common/Vector3.h"

using namespace NCL;
using namespace CSC8508;

CameraComponent* CameraComponent::mainCamera = nullptr;


void CameraComponent::SetPitch(float pitch) { 
	Vector3 rotation = transform->GetOrientation().ToEuler();
	rotation.x = pitch;
	transform->SetOrientation(Quaternion::EulerAnglesToQuaternion(rotation.x, rotation.y, rotation.z));
	camera->SetPitch(pitch); 
}
void CameraComponent::SetYaw(float yaw) { 
	Vector3 rotation = transform->GetOrientation().ToEuler();
	rotation.y = yaw;
	transform->SetOrientation(Quaternion::EulerAnglesToQuaternion(rotation.x, rotation.y, rotation.z));
	camera->SetYaw(yaw);
}

void CameraComponent::SetNearPlane(float nearPlane) { camera->SetNearPlane(nearPlane); }
void CameraComponent::SetFarPlane(float farPlane) { camera->SetFarPlane(farPlane); }

void CameraComponent::SetPosition(const Maths::Vector3& position) {
	camera->SetPosition(position);
	transform->SetPosition(position);
}

CameraComponent::CameraComponent(GameObject* object, bool main) : Component(object) {

	if (mainCamera == nullptr || main)
		mainCamera = this;

	Vector3 rotation = transform->GetOrientation().ToEuler();

	camera =  new Camera(rotation.x, rotation.y, transform->GetPosition());
}

CameraComponent::~CameraComponent() {
	if (mainCamera == this)
		mainCamera = nullptr;

	delete camera;
}

void CameraComponent::Update(float dt) {
	camera->SetPosition(transform->GetPosition());
	
	Vector3 rotation = transform->GetOrientation().ToEuler();
	camera->SetPitch(rotation.x);
	camera->SetYaw(rotation.y);
}