#pragma once
#include "../Engine/Component.h"

#include "../../Common/Vector3.h"

namespace NCL {

	class Camera;

	namespace CSC8508 {

		class PhysicsObject;
		class Game;

		class PlayerComponent : public Component {

		public:
			PlayerComponent(GameObject* object, Game* game);
			void Update(float dt);
			void UpdateControls(float dt);
			void OnCollisionBegin(GameObject* otherObject) override;
			void OnCollisionStay(GameObject* otherObject) override;

		private:
			Maths::Vector3 dir;
			PhysicsObject* physicsObject;

			bool lockOrientation;

			float yaw;
			float pitch;
			float cameraDistance;

			float speed;

			float jump;
			float lastCollisionTimer;

			Game* game;
			Camera* camera;

			void CameraMovement();
			void Movement();
			void Jump();

		};

	}
}