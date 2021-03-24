#include "Shadow.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"



// 阴影的分辨率
#define SHADOWSIZE 2048


Shadow::Shadow()
{
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glGenTextures(1, &depth2Dmap);
	glBindTexture(GL_TEXTURE_2D, depth2Dmap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "帧缓冲区未完成编译!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}


//获取帧缓冲立方体贴图
unsigned int Shadow::getCubemapTexture()
{
	return depthCubemap;
}

unsigned int Shadow::get2DmapTexture()
{
	return depth2Dmap;
}

void Shadow::BindShadowFBOAsCubemap()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
}

void Shadow::BindShadowFBOAs2D()
{
	glBindTexture(GL_TEXTURE_2D, depth2Dmap);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth2Dmap, 0);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
}

// 只能绘制点光源的阴影
void Shadow::DrawPointLightShadowMap(NCL::Rendering::OGLShader* shadowshader, PointLight& pointlight, int index)
{
	if (pointlight.getPointNumber() < index + 1)
	{
		std::cout << "错误，无法找到对应的第" << index << "个点光源数据" << std::endl;
		return;
	}


	glUseProgram(shadowshader->GetProgramID());

	glm::vec3 lightPos = pointlight.getPos()[index];

	float farPlane = pointlight.getFarPlane();

	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
	for (GLuint i = 0; i < 6; i++)
		glUniformMatrix4fv(glGetUniformLocation(shadowshader->GetProgramID(), ("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE, glm::value_ptr(shadowTransforms[i]));
	glUniform1f(glGetUniformLocation(shadowshader->GetProgramID(), "farPlane"), farPlane);
	glUniform3fv(glGetUniformLocation(shadowshader->GetProgramID(), "lightPos"), 1, &lightPos[0]);

}

void Shadow::DrawSpotLightShadowMap(NCL::Rendering::OGLShader* shadowshader, SpotLight& spotlight, int index)
{
	if (spotlight.getSpotNumber() < index + 1)
	{
		std::cout << "错误，无法找到对应的第" << index << "个聚光灯光源数据" << std::endl;
		return;
	}

	auto lightPos = spotlight.getPos()[index];

	glUseProgram(shadowshader->GetProgramID());

	float farPlane = spotlight.getFarPlane();

	glUniform1f(glGetUniformLocation(shadowshader->GetProgramID(), "farPlane"), farPlane);
	glUniform3fv(glGetUniformLocation(shadowshader->GetProgramID(), "lightPos"), 1, &lightPos[0]);

}
