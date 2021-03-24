#pragma once

#include "../../include/glm/glm.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"
#include "../../include/glm/gtc/type_ptr.hpp"
#include <vector>

namespace NCL {

	namespace Rendering {
		class OGLShader;
	}

	namespace CSC8508 {

		class PointLight
		{

		public:
			PointLight(std::vector<glm::vec3>& position);
			void render(NCL::Rendering::OGLShader* lightshader);
			int getPointNumber();
			std::vector<glm::vec3> getPos();
			float getFarPlane();
		protected:
			std::vector<glm::vec3> position;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;

			float constant;
			float linear;
			float quadratic;
			float farPlane = 2000.0f;
		};
	}
}


