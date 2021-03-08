#define NOMINMAX

#include "PlayerComponent.h"
#include "Game.h"
#include "../Engine/GameObject.h"
#include "../Engine/GameWorld.h"
#include "../../Common/Camera.h"
#include "../../Common/Window.h"

using namespace NCL;
using namespace CSC8508;

PlayerComponent::PlayerComponent(GameObject* object, Game* game) : Component(object) {

	speed = 10.0f;
	pitch = 20.0f;
	yaw = 0.0f;
	cameraDistance = 10.0f;
	lastCollisionTimer = 0.0f;
	//lockOrientation = false;
	physicsObject = object->GetPhysicsObject();

	camera = game->GetWorld()->GetMainCamera();
	this->game = game;
};

void PlayerComponent::Update(float dt) {
	//lastCollisionTimer += dt;

	physicsObject->SetAngularVelocity(Vector3(0, 0, 0));

	//Update player object, lock orientation.
	Quaternion orientation = Quaternion::EulerAnglesToQuaternion(0, yaw, 0);
	transform->SetOrientation(orientation);

	UpdateControls(dt);
}

void PlayerComponent::OnCollisionBegin(GameObject* otherObject) {
	//lastCollisionTimer = 0.0f;
	
}

void PlayerComponent::UpdateControls(float dt) {

	//Receive input
	pitch += (Window::GetMouse()->GetRelativePosition().y);
	pitch = std::max(-15.0f, std::min(90.0f, pitch));
	yaw -= Window::GetMouse()->GetRelativePosition().x;

	if (Window::GetMouse()->WheelMoved()) {
		cameraDistance -= Window::GetMouse()->GetWheelMovement();
		cameraDistance = std::max(5.0f, cameraDistance);
	}

	Quaternion orientation = Quaternion::EulerAnglesToQuaternion(0, yaw, 0);

	//Update camera
	Vector3 angles = orientation.ToEuler();

	camera->SetPitch(-pitch);
	camera->SetYaw(angles.y);

	Quaternion cameraAngle = Quaternion::EulerAnglesToQuaternion(-pitch, angles.y, 0.0f);
	Vector3 cameraOffset = cameraAngle * (Vector3(0, 0, 1) * cameraDistance);
	Vector3 cameraFocusPoint = transform->GetPosition() + Vector3(0, 2, 0);
	camera->SetPosition(cameraFocusPoint + cameraOffset);

	//Update movement
	if (lastCollisionTimer < 0.1f ) {
		Vector3 direction = Vector3(0, 0, 0);
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
			direction += Vector3(0, 0, -1);
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
			direction += Vector3(0, 0, 1);
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
			direction += Vector3(-1, 0, 0);
		}
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
			direction += Vector3(1, 0, 0);
		}

		physicsObject->body->addForce(orientation * direction.Normalised() * speed);
	}

}


