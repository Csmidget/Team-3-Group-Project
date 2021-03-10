#include "PointLight.h"


PointLight::PointLight(std::vector<glm::vec3>& position) :
	position(position),
	ambient(glm::vec3(1.0)),
	diffuse(glm::vec3(1.0)),
	specular(glm::vec3(1.0)),
	constant(0.09),
	linear(0.09),
	quadratic(0.032)
{

}

void PointLight::render(NCL::Rendering::OGLShader* lightshader)
{
	for (int index = 0; index < (position.size()); index++)
	{
		glUniform3fv(glGetUniformLocation(lightshader->GetProgramID(), ("pointLights[" + std::to_string(index) + "].position").c_str()), 1, &position[index][0]);
		glUniform3fv(glGetUniformLocation(lightshader->GetProgramID(), ("pointLights[" + std::to_string(index) + "].ambient").c_str()), 1, &ambient[0]);
		glUniform3fv(glGetUniformLocation(lightshader->GetProgramID(), ("pointLights[" + std::to_string(index) + "].diffuse").c_str()), 1, &diffuse[0]);
		glUniform3fv(glGetUniformLocation(lightshader->GetProgramID(), ("pointLights[" + std::to_string(index) + "].specular").c_str()), 1, &specular[0]);

		glUniform1f(glGetUniformLocation(lightshader->GetProgramID(), ("pointLights[" + std::to_string(index) + "].constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(lightshader->GetProgramID(), ("pointLights[" + std::to_string(index) + "].linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(lightshader->GetProgramID(), ("pointLights[" + std::to_string(index) + "].quadratic").c_str()), quadratic);
		glUniform1f(glGetUniformLocation(lightshader->GetProgramID(), ("pointLights[" + std::to_string(index) + "].farPlane").c_str()), farPlane);
	}

}

int PointLight::getPointNumber()
{
	return position.size();
}

std::vector<glm::vec3> PointLight::getPos()
{
	return position;
}

float PointLight::getFarPlane()
{
	return farPlane;
}
