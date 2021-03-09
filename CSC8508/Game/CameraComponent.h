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
			~CameraComponent() override;

			void SetPitch(float pitch);
			void SetYaw(float yaw);
			void SetNearPlane(float nearPlane);
			void SetFarPlane(float farPlane);
			void SetPosition(const Maths::Vector3& position);

			static CameraComponent* GetMain() { return mainCamera; }
			void Update(float dt) override;

		private:
			static CameraComponent* mainCamera;
			Camera* camera;
		};
	}
}