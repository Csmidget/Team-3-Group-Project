#pragma once
#include "../Engine/Component.h"
#include "../../Common/Vector3.h"

namespace NCL {

	class Camera;

	namespace CSC8508 {

		namespace Audio {
			class SoundInstance;
		}

		class PhysicsObject;
		class Game;
		class CameraComponent;
		class LocalNetworkPlayerComponent;

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

			void fixedUpdate(float dt);
			void UpdateControls(float dt);
			void Start() override;
			void Update(float dt) override;
			void OnCollisionBegin(GameObject* otherObject) override;
			void OnCollisionStay(GameObject* otherObject) override;
			void OnCollisionEnd(GameObject* otherObject) override;

			PlayerMovementState GetCurrentMovementState()
			{
				return movementState;
			}
		
		protected:
			std::vector<std::string> DebugInfo() override;

		private:

			PlayerMovementState movementState;
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

			const float MAX_WALKING_SPEED = 6.0f;
			const float MAX_ACCELERATION = 100.f;
			const float MAX_DECELERATION = 80.f;

			bool recquestedJump;
			const float JUMP = 7.5f;
			int jumpCounter;
			float lastCollisionTimer;

		
			
			Game* game;
			CameraComponent* camera;
			LocalNetworkPlayerComponent* lnpc;

			PlayerMovementState IdleOrRunning();
			void CameraMovement();
			void PlayerInputs();
			void Jump();
			void AccelerateTo(Maths::Vector3 targetVelocity, float dt);

		};

	}
}