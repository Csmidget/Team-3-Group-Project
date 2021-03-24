#define NOMINMAX

#include "PlayerComponent.h"

#include "LocalNetworkPlayerComponent.h"
#include "GameStateManagerComponent.h"
#include "Game.h"
#include "CameraComponent.h"
#include "PlayerAnimComponent.h"
#include "ScoreComponent.h"
#include "BonusComponent.h"
#include "RingComponent.h"

#include "../Engine/GameObject.h"
#include "../Engine/GameWorld.h"
#include "../Audio/SoundManager.h"
#include "../Audio/SoundInstance.h"

#include <algorithm>

using namespace NCL;
using namespace CSC8508;
using namespace Maths;

PlayerComponent::PlayerComponent(GameObject* object, Game* game) : Component("PlayerComponent", object) 
{
	movementState = PlayerMovementState::WALKING;
	receiveInputs = true;

	recquestedJump = false;
	jump = 7.5f;
	//jump = 110.f;

	MAX_WALKING_SPEED = 6.0f;
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

	gameObject->AddComponent<PlayerAnimComponent>(game);

	this->game = game;
}

void PlayerComponent::Start() {
	lnpc = game->GetWorld()->GetComponentOfType<LocalNetworkPlayerComponent>();
}

void PlayerComponent::fixedUpdate(float dt) {
	lastCollisionTimer += dt;

	currentVelocity = physicsObject->body->getLinearVelocity();

	AccelerateTo(direction * MAX_WALKING_SPEED, dt);
	Jump();
}

void PlayerComponent::Update(float dt) {

	Quaternion orientation = Quaternion::EulerAnglesToQuaternion(0, yaw, 0);
	transform->SetOrientation(orientation);

	if (receiveInputs)
	{
		UpdateControls(dt);
	}

	if (lnpc)
		lnpc->SetTransform(transform->GetPosition(), transform->GetOrientation());
}

void PlayerComponent::OnCollisionBegin(GameObject* otherObject)
{
	movementState = IdleOrRunning();
	lastCollisionTimer = 0.0f;

	if (!otherObject->IsActive()) return;

	ScoreComponent* score = ScoreComponent::instance;

	if (score) {
		if (otherObject->HasTag("Bonus")) {
			score->AddScore(otherObject->GetComponent<BonusComponent>()->GetBonus());
			otherObject->OnKill();
			return;
		}

		if (otherObject->HasTag("Ring"))
		{
			score->AddScore(otherObject->GetComponent<RingComponent>()->GetBonus());
			otherObject->OnKill();
			return;
		}

		if (otherObject->HasTag("Goal")) GameStateManagerComponent::instance->SetPlayerFinished(true);

	}
}

void NCL::CSC8508::PlayerComponent::OnCollisionStay(GameObject* otherObject)
{
	movementState = IdleOrRunning();
	lastCollisionTimer = 0.0f;
}

void NCL::CSC8508::PlayerComponent::OnCollisionEnd(GameObject* otherObject)
{
	movementState = PlayerMovementState::JUMP_ONE;

}

PlayerMovementState NCL::CSC8508::PlayerComponent::IdleOrRunning()
{
	if (currentVelocity.Length() <= 1.0f)
	{
		return PlayerMovementState::IDLE;
	}
	else {
		return PlayerMovementState::WALKING;
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

void NCL::CSC8508::PlayerComponent::PlayerInputs()
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
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE) && movementState != PlayerMovementState::JUMP_TWO)
	{
		recquestedJump = true;
	}
	direction = transform->GetOrientation() * direction.Normalised();
}

void NCL::CSC8508::PlayerComponent::Jump()
{

	if (movementState != PlayerMovementState::JUMP_TWO)
	{
		if (recquestedJump)
		{
			recquestedJump = false;
			JumpSound->Play();
			movementState = (movementState == PlayerMovementState::JUMP_ONE ? PlayerMovementState::JUMP_TWO : PlayerMovementState::JUMP_ONE);
			jumpCounter = 3;
		}
		if (jumpCounter > 0)
		{
			physicsObject->body->addImpulse(transform->GetOrientation() * Vector3(0, 1, 0) * jump);
			jumpCounter--;
		}

	}

}


void NCL::CSC8508::PlayerComponent::AccelerateTo(Vector3 targetVelocity, float dt)
{
	Vector3 currentVelocityXZ = Vector3(currentVelocity.x, 0, currentVelocity.z);
	Vector3 delta = targetVelocity - currentVelocityXZ;
	Vector3 deltaAccel = delta / dt;
	float accelMag = std::min(deltaAccel.Length(), (Vector3::Dot(currentVelocityXZ, delta) > 0 ? MAX_ACCELERATION : MAX_DECELERATION));
	Vector3 acceleration = deltaAccel.Normalised() * accelMag;

	physicsObject->body->addForce(acceleration * physicsObject->body->returnBody()->getMass());
}



void PlayerComponent::UpdateControls(float dt) 
{
	CameraMovement();
	PlayerInputs();
}

