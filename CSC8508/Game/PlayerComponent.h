#pragma once
#include "../Engine/Component.h"
#include "../../Common/Vector3.h"
#include"../Audio/SoundInstance.h"

namespace NCL {

	class Camera;

	namespace CSC8508 {

		class PhysicsObject;
		class Game;
		class CameraComponent;

		enum PlayerMovementState
		{
			WALKING,
			JUMP_ONE,
			JUMP_TWO
		};


		class PlayerComponent : public Component {

		public:
			PlayerComponent(GameObject* object, Game* game);
			
			//Testing constructor
			PlayerComponent(GameObject* object);
			

			void Update(float dt);
			void UpdateControls(float dt);
			void OnCollisionBegin(GameObject* otherObject) override;
			void OnCollisionStay(GameObject* otherObject) override;
			void OnCollisionEnd(GameObject* otherObject) override;

			void SetSpeed(float speed, float max) { this->speed = speed; this->MAX_WALKING_SPEED = max; }
			void SetJump(float jump, float max) { this->jump = jump; this->MAX_AIR_SPEED = max;
			}
		private:

			PlayerMovementState movementState;
			//Maths::Vector3 dir;
			Maths::Vector3 direction;
			PhysicsObject* physicsObject;
			Audio::SoundInstance* JumpSound;

			bool lockOrientation;

			float yaw;
			float pitch;
			float cameraDistance;
			const float MAX_CAMERA_DISTANCE = 18.f;
			const float MIN_CAMERA_DISTANCE = 13.f;

			float speed;
			float MAX_WALKING_SPEED;
			const float MAX_ACCELERATION = 1000000000000.f;
			const float MAX_DECELERATION = 100000000000000.f;


			float jump;
			float MAX_AIR_SPEED;
			int jumpCounter;
			bool jumping;
			float lastCollisionTimer;

			bool testing;
			float testTimer;
			bool hasJumped;
			
			Game* game;
			CameraComponent* camera;
			

			void CameraMovement();
			void Movement();
			void Jump();
			void ClampVelocity();
			void AccelerateTo(Maths::Vector3 targetVelocity, float dt);

			void Interact();

			void Testing();
			void TestMovement();
			void TestStaticJumping();
			void TestRunningJump();
		};

	}
}