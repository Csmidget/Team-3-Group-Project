#include "SpotLight.h"
#include "GameTechRenderer.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"

using namespace NCL;
using namespace CSC8508;

SpotLight::SpotLight(std::vector<glm::vec3>& position) :
	position(position),
	direct(glm::vec3(0.0f, -1.0f, -0.5f)),
	ambient(glm::vec3(8.0)),
	diffuse(glm::vec3(5.0)),
	specular(glm::vec3(7.0)),
	constant(0.09),
	linear(0.09),
	quadratic(0.032)
{

}

void SpotLight::render(NCL::Rendering::OGLShader* lightshader)
{
	int i = 0;
	for (int i = 0; i < position.size(); i++)
	{
		glUniform3fv(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].position").c_str()), 1, &position[i][0]);
		glUniform3fv(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].direction").c_str()), 1, &direct[0]);
		glUniform3fv(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].ambient").c_str()), 1, &ambient[0]);
		glUniform3fv(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].diffuse").c_str()), 1, &diffuse[0]);
		glUniform3fv(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].specular").c_str()), 1, &specular[0]);
		glUniform1f(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].constant").c_str()), constant);
		glUniform1f(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].linear").c_str()), linear);
		glUniform1f(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].quadratic").c_str()), quadratic);
		glUniform1f(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].cutOff").c_str()), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].outerCutOff").c_str()), glm::cos(glm::radians(15.0f)));
		glUniform1f(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].farPlane").c_str()), farPlane);
		glUniformMatrix4fv(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].shadowVP").c_str()), 1, false, glm::value_ptr(getLightVPMatrix(i)));
	}
}

int SpotLight::getSpotNumber()
{
	return position.size();
}

std::vector<glm::vec3> SpotLight::getPos()
{
	return position;
}

float SpotLight::getFarPlane()
{
	return farPlane;
}

glm::mat4 SpotLight::getLightVPMatrix(int idx)
{
	glm::vec3 f = direct;
	glm::vec3 u = glm::cross(f, glm::vec3(f.y, f.z, f.x));

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);

	//glm::mat4 shadowProj = glm::perspective(glm::radians(35.0f), 1.0f, 0.1f, farPlane);
	//glm::mat4 shadowView = glm::lookAt(position[idx], position[idx] + glm::normalize(f), glm::normalize(u));
	glm::mat4 shadowView = glm::lookAt(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
	return shadowProj * shadowView;
}
