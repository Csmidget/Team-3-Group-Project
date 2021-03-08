#define NOMINMAX

#include "PlayerComponent.h"
#include "Game.h"
#include "../Engine/GameObject.h"
#include "../Engine/GameWorld.h"
#include "../../Common/Camera.h"
#include "../../Common/Window.h"
#include "../../Common/Maths.h"
#include <algorithm>

using namespace NCL;
using namespace CSC8508;
using namespace Maths;

PlayerComponent::PlayerComponent(GameObject* object, Game* game) : Component(object) 
{
	movementState = PlayerMovementState::WALKING;

	speed = 50.0f;
	jump = 150.f;

	MAX_WALKING_SPEED = 1000.f;
	MAX_AIR_SPEED = 10000.f;

	jumpCounter = 0;

	pitch = 20.0f;
	yaw = 0.0f;
	cameraDistance = 10.0f;
	jumping = false;
	lastCollisionTimer = 0.f;
	lockOrientation = false;
	physicsObject = object->GetPhysicsObject();

	testing = false;
	testTimer = 0.f;
	hasJumped = false;

	//physicsObject->SetFriction(0.9f);
	physicsObject->body->setDamping(0.5, 0.f);
	camera = game->GetWorld()->GetMainCamera();

	this->game = game;
}
NCL::CSC8508::PlayerComponent::PlayerComponent(GameObject* object) : Component(object)
{
	movementState = PlayerMovementState::WALKING;
	
	speed = 50.0f;
	jump = 100.f;

	MAX_WALKING_SPEED = 1000.f;
	MAX_AIR_SPEED = 10000.f;

	jumpCounter = 0;
	
	pitch = 20.0f;
	yaw = 0.0f;
	cameraDistance = 10.0f;
	jumping = false;
	lastCollisionTimer = 0.1f;
	lockOrientation = false;
	physicsObject = object->GetPhysicsObject();

	testing = true;
	testTimer = 0.f;
	hasJumped = false;

	camera = nullptr;
	game = nullptr;
}


void PlayerComponent::Update(float dt) {
	lastCollisionTimer += dt;

	physicsObject->SetAngularVelocity(Vector3(0, 0, 0));

	Quaternion orientation = Quaternion::EulerAnglesToQuaternion(0, yaw, 0);
	transform->SetOrientation(orientation);

	if (camera != nullptr)
	{
		UpdateControls(dt);
	}
	else
	{
		testTimer += dt;
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM0))
		{
			testing != testing;
		}
		Testing();
	}

	//std::cout << movementState << std::endl;
	//ClampVelocity();
}

void PlayerComponent::OnCollisionBegin(GameObject* otherObject)
{
	movementState = PlayerMovementState::WALKING;
	lastCollisionTimer = 0.0f;
	jumping = false;
}

void NCL::CSC8508::PlayerComponent::OnCollisionStay(GameObject* otherObject)
{

}

void NCL::CSC8508::PlayerComponent::OnCollisionEnd(GameObject* otherObject)
{
	hasJumped = true;
	jumping = true;
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
	Vector3 direction = Vector3(0, 0, 0);
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W))
	{
		direction += Vector3(0, 0, -1);
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S))
	{
		direction += Vector3(0, 0, 1);
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A))
	{
		direction += Vector3(-1, 0, 0);
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D))
	{
		direction += Vector3(1, 0, 0);
	}
	
	// ----------- MIGHT NEED TO CHANGE ---------------
	lastCollisionTimer < 0.1f ? speed = 50 : speed = 10;

	physicsObject->AddForce(transform->GetOrientation() * direction.Normalised() * speed);
}

void NCL::CSC8508::PlayerComponent::Jump()
{
	//if (lastCollisionTimer < 0.05f && Window::GetKeyboard()->KeyDown(KeyboardKeys::SPACE))

	if (movementState != PlayerMovementState::JUMP_TWO)
	{

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE))
		{
			movementState = (PlayerMovementState)(movementState + 1);
			jumpCounter = 3;
		}
		if (jumpCounter > 0)
		{
			//std::cout << "Jumping" << std::endl;
			physicsObject->AddForce(transform->GetOrientation() * Vector3(0, 1, 0) * jump * jumpCounter);
			//std::cout << physicsObject->GetForce() << std::endl;
			jumpCounter--;
		}

	}

}

void NCL::CSC8508::PlayerComponent::ClampVelocity()
{
	//Seperate out into walking and jumping
	Vector3 currentVelocity = physicsObject->GetLinearVelocity();
	float magnitude = Clamp(currentVelocity.Length(), 0.f ,jumping ? MAX_AIR_SPEED : MAX_WALKING_SPEED);

	physicsObject->SetLinearVelocity(currentVelocity.Normalised() * magnitude);
}

void NCL::CSC8508::PlayerComponent::Stop()
{
}

void NCL::CSC8508::PlayerComponent::Interact()
{
}

void PlayerComponent::UpdateControls(float dt) 
{
	CameraMovement();
	Movement();
	Jump();
}

void NCL::CSC8508::PlayerComponent::Testing()
{
	if (testing)
	{
		TestMovement(); // around 50
		//TestStaticJumping(); // around 200
		//TestRunningJump();
	}
}

void NCL::CSC8508::PlayerComponent::TestMovement()
{
	physicsObject->AddForce(transform->GetOrientation() * Vector3(0, 0, 1) * speed);
}

void NCL::CSC8508::PlayerComponent::TestStaticJumping()
{
	if (!jumping)
	{
		testTimer = 0.f;
		physicsObject->AddForce(transform->GetOrientation() * Vector3(0, 1, 0) * jump);
	}
}

void NCL::CSC8508::PlayerComponent::TestRunningJump()
{
	if (testTimer < 2)
	{
		physicsObject->AddForce(transform->GetOrientation() * Vector3(0, 0, 1) * speed);
	}
	else if (!hasJumped)
	{
		physicsObject->AddForce(transform->GetOrientation() * Vector3(0, 1, 0) * jump);
	}
}
