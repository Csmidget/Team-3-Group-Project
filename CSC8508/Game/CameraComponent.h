#pragma once
#include "../Engine/Component.h"

namespace NCL {

	class Camera;

	namespace Maths {
		class Vector3;
	}

	namespace CSC8508 {

		class CameraComponent : public Component {

		public:
			const Camera* GetCamera() const { return camera; }
			CameraComponent(GameObject* object, bool main = false);
			~CameraComponent();

			float GetPitch();
			void SetPitch(float pitch);
			float GetYaw();
			void SetYaw(float yaw);
			void SetNearPlane(float nearPlane);
			void SetFarPlane(float farPlane);
			Maths::Vector3 GetPosition();
			void SetPosition(const Maths::Vector3& position);

			static CameraComponent* GetMain() { return mainCamera; }
			static void SetMain(CameraComponent* cam) {
				if (!cam)
					return;

				mainCamera = cam;
			}

			void Update(float dt) override;

		private:
			static CameraComponent* mainCamera;
			Camera* camera;
		};
	}
}