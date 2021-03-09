#pragma once

#include <../../Assets/packages/glm.0.9.9.800/build/native/include/glm/glm.hpp>
#include <../../Assets/packages/glm.0.9.9.800/build/native/include/glm/gtc/matrix_transform.hpp>
#include <../../Assets/packages/glm.0.9.9.800/build/native/include/glm/gtc/type_ptr.hpp>
#include "GameTechRenderer.h"


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
	float farPlane = 10.0f;
};

