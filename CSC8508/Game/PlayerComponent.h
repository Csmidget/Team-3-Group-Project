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
			JUMP_TWO,
			IDLE
		};


		class PlayerComponent : public Component {

		public:
			PlayerComponent(GameObject* object, Game* game);
		
			bool receiveInputs;

			void Update(float dt);
			void UpdateControls(float dt);
			void OnCollisionBegin(GameObject* otherObject) override;
			void OnCollisionStay(GameObject* otherObject) override;
			void OnCollisionEnd(GameObject* otherObject) override;

			PlayerMovementState GetCurrentMovementState()
			{
				return movementState;
			}
		
		private:

			PlayerMovementState movementState;
			//Maths::Vector3 dir;
			Maths::Vector3 direction;
			Maths::Vector3 currentVelocity;
			PhysicsObject* physicsObject;
			Audio::SoundInstance* JumpSound;

			bool lockOrientation;

			float yaw;
			float pitch;
			float cameraDistance;
			const float MAX_CAMERA_DISTANCE = 18.f;
			const float MIN_CAMERA_DISTANCE = 13.f;

			float MAX_WALKING_SPEED;
			const float MAX_ACCELERATION = 100.f;
			const float MAX_DECELERATION = 80.f;


			float jump;
			float MAX_AIR_SPEED;
			int jumpCounter;
			float lastCollisionTimer;

		
			
			Game* game;
			CameraComponent* camera;
			
			void IdleCheck();
			void CameraMovement();
			void Movement();
			void Jump();
			void AccelerateTo(Maths::Vector3 targetVelocity, float dt);

		};

	}
}