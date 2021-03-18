#define NOMINMAX

#include "PlayerComponent.h"
#include "Game.h"
#include "../Engine/GameObject.h"
#include "../Engine/GameWorld.h"
#include "../../Common/Camera.h"
#include "../../Common/Window.h"
#include "../../Common/Maths.h"
#include "CameraComponent.h"

#include"../Audio/SoundManager.h"
#include <algorithm>

using namespace NCL;
using namespace CSC8508;
using namespace Maths;

PlayerComponent::PlayerComponent(GameObject* object, Game* game) : Component("PlayerComponent", object) 
{
	movementState = PlayerMovementState::WALKING;
	receiveInputs = true;

	jump = 110.f;

	MAX_WALKING_SPEED = 10.f;
	MAX_AIR_SPEED = 10000.f;

	jumpCounter = 0;

	pitch = 20.0f;
	yaw = 0.0f;
	cameraDistance = 10.0f;

	lastCollisionTimer = 0.f;
	lockOrientation = false;
	physicsObject = object->GetPhysicsObject();

	camera = CameraComponent::GetMain();

	//Audio
	JumpSound = new Audio::SoundInstance();
	Audio::SoundManager::CreateInstance("jumpSound.wav", JumpSound);
	JumpSound->Set3D(false);
	JumpSound->SetVolume(0.4f);

	this->game = game;
}



void PlayerComponent::Update(float dt) {
	lastCollisionTimer += dt;

	currentVelocity = physicsObject->body->getLinearVelocity();
	physicsObject->SetAngularVelocity(Vector3(0, 0, 0));

	Quaternion orientation = Quaternion::EulerAnglesToQuaternion(0, yaw, 0);
	transform->SetOrientation(orientation);

	IdleCheck();
	if (receiveInputs)
	{
		UpdateControls(dt);
	}
}

void PlayerComponent::OnCollisionBegin(GameObject* otherObject)
{
	movementState = PlayerMovementState::WALKING;
	lastCollisionTimer = 0.0f;

}

void NCL::CSC8508::PlayerComponent::OnCollisionStay(GameObject* otherObject)
{
	movementState = PlayerMovementState::WALKING;
	lastCollisionTimer = 0.0f;
}

void NCL::CSC8508::PlayerComponent::OnCollisionEnd(GameObject* otherObject)
{
	movementState = PlayerMovementState::JUMP_ONE;

}

void NCL::CSC8508::PlayerComponent::IdleCheck()
{
	if (currentVelocity.Length() <= 0.1f)
	{
		movementState = PlayerMovementState::IDLE;
	}
}

void NCL::CSC8508::PlayerComponent::CameraMovement()
{
	//Receive input
	pitch += (Window::GetMouse()->GetRelativePosition().y);
	pitch = std::max(0.0f, std::min(90.0f, pitch));
	yaw -= Window::GetMouse()->GetRelativePosition().x;

	cameraDistance += (Window::GetMouse()->GetRelativePosition().y);
	cameraDistance = std::clamp(cameraDistance, MIN_CAMERA_DISTANCE, MAX_CAMERA_DISTANCE);

	Quaternion orientation = Quaternion::EulerAnglesToQuaternion(0, yaw, 0);

	//Update camera
	Vector3 angles = orientation.ToEuler();

	camera->SetPitch(-pitch);
	camera->SetYaw(angles.y);
	Quaternion cameraAngle = Quaternion::EulerAnglesToQuaternion(-pitch, angles.y, 0.0f);
	Vector3 cameraOffset = cameraAngle * (Vector3(0, 0, 1) * cameraDistance);
	Vector3 cameraFocusPoint = transform->GetPosition() + Vector3(0, 2, 0);

	//Build ray from character camera if collision then move camera to ray position
	camera->SetPosition(cameraFocusPoint + cameraOffset);
}

void NCL::CSC8508::PlayerComponent::Movement()
{
	//Update movement
	direction = Vector3(0, 0, 0);
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::W))
	{
		direction += Vector3(0, 0, -1);
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::S))
	{
		direction += Vector3(0, 0, 1);
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::A))
	{
		direction += Vector3(-1, 0, 0);
	}
	if (Window::GetKeyboard()->KeyHeld(KeyboardKeys::D))
	{
		direction += Vector3(1, 0, 0);
	}
	
	direction = transform->GetOrientation() * direction.Normalised();
}

void NCL::CSC8508::PlayerComponent::Jump()
{
	//if (lastCollisionTimer < 0.05f && Window::GetKeyboard()->KeyDown(KeyboardKeys::SPACE))

	if (movementState != PlayerMovementState::JUMP_TWO)
	{

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE))
		{
			JumpSound->Play();
			movementState = (movementState == PlayerMovementState::JUMP_ONE ? PlayerMovementState::JUMP_TWO : PlayerMovementState::JUMP_ONE);
			Vector3 currentForce = physicsObject->body->getForce();
			physicsObject->body->clearForces();
			physicsObject->body->addForce(Vector3(currentForce.x, 0, currentForce.z));
			jumpCounter = 3;
		}
		if (jumpCounter > 0)
		{
			//std::cout << "Jumping" << std::endl;
			physicsObject->body->addForce(transform->GetOrientation() * Vector3(0, 1, 0) * jump * 3);
			//std::cout << physicsObject->GetForce() << std::endl;
			jumpCounter--;
		}

	}

}


void NCL::CSC8508::PlayerComponent::AccelerateTo(Vector3 targetVelocity, float dt)
{
	Vector3 currentVelocityXZ = Vector3(currentVelocity.x, 0, currentVelocity.z);
	Vector3 delta = targetVelocity - currentVelocityXZ;
	Vector3 deltaAccel = (targetVelocity - currentVelocityXZ) / dt;
	float accelMag = std::min(deltaAccel.Length(), (Vector3::Dot(currentVelocityXZ, delta) > 0 ? MAX_ACCELERATION : MAX_DECELERATION));

	Vector3 acceleration = deltaAccel.Normalised() * accelMag;
	physicsObject->body->addForce(acceleration / physicsObject->GetInverseMass()  );
}



void PlayerComponent::UpdateControls(float dt) 
{
	CameraMovement();
	Movement();

	AccelerateTo(direction * MAX_WALKING_SPEED, dt);

	Jump();
}

