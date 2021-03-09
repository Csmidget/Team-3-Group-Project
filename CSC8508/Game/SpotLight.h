#pragma once

#include <../../Assets/packages/glm.0.9.9.800/build/native/include/glm/glm.hpp>
#include <../../Assets/packages/glm.0.9.9.800/build/native/include/glm/gtc/matrix_transform.hpp>
#include <../../Assets/packages/glm.0.9.9.800/build/native/include/glm/gtc/type_ptr.hpp>
#include "GameTechRenderer.h"


//spotlight class

class SpotLight
{
public:
	SpotLight(std::vector<glm::vec3>& position);
	void render(NCL::Rendering::OGLShader* lightshader);

private:
	std::vector<glm::vec3> position;
	glm::vec3 direct;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

};

