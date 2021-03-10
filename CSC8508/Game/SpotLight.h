#pragma once

#include "../../include/glm/glm.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"
#include "../../include/glm/gtc/type_ptr.hpp"
#include "GameTechRenderer.h"


class SpotLight
{
public:
	SpotLight(std::vector<glm::vec3>& position);
	void render(NCL::Rendering::OGLShader* lightshader);

	int getSpotNumber();
	std::vector<glm::vec3> getPos();
	float getFarPlane();
	glm::vec3 getDirect() { return direct; }

	glm::mat4 getLightVPMatrix(int idx);

private:
	std::vector<glm::vec3> position;
	glm::vec3 direct;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
	float farPlane = 50.0f;

};

