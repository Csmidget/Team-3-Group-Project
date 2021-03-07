#pragma once
#include"../../Common/Vector3.h"
#include"../../Common/Vector4.h"
#include"../Game/GameTechRenderer.h"
#include "../Engine/GameWorld.h"

#include "../../Plugins/OpenGLRendering/OGLRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"

#include "../../Common/ResourceManager.h"


namespace NCL {
	class Maths::Vector3;
	class Maths::Vector4;

	namespace CSC8508 {
		class Light
		{
		public:
			Light() {}
			Light(const Vector3& position, const Vector4& colour, float radius)
			{
				this->position = position;
				this->colour = colour;
				this->radius = radius;
			}

			~Light(void) {}

			Vector3 GetPosition() const { return position; }
			void SetPosition(const Vector3& val) { position = val; }

			float GetRadius() const { return radius; }
			void SetRadius(float val) { radius = val; }

			Vector4 GetColour() const { return colour; }
			void SetColour(const Vector4& val) { colour = val; }


			void UpdateLight(float dt)
			{
				Vector3 forward = Vector3(0, 0, -1);
				Vector3 right = Vector3(1, 0, 0);

				float speed = 20 * dt;

				if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::I))
					position += forward * speed;
				if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::K))
					position -= forward * speed;
				if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::J))
					position -= right * speed;
				if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L))
					position += right * speed;
				if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::O))
					position.y += speed;
				if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::P))
					position.y -= speed;

			}

		protected:
			Vector3 position;
			float radius;
			Vector4 colour;


		};









	}

}




