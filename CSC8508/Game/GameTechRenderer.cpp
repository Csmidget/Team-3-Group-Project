#include "GameTechRenderer.h"
#include "CameraComponent.h"
#include "../Engine/GameWorld.h"
#include "../Engine/GameObject.h"
#include "../../Common/Camera.h"
#include "../../Common/Vector2.h"
#include "../../Common/Vector3.h"
#include "../../Common/TextureLoader.h"
#include "../../Common/Maths.h"
#include "../../Common/MeshAnimation.h"
#include "../../Common/ResourceManager.h"

#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"

#include "../../include/glm/glm.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"
#include "../../include/glm/gtc/type_ptr.hpp"
#include "PointLight.h"
#include "SpotLight.h"
#include "Shadow.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8508;



Matrix4 biasMatrix = Matrix4::Translation(Vector3(0.5, 0.5, 0.5)) * Matrix4::Scale(Vector3(0.5, 0.5, 0.5));



GameTechRenderer::GameTechRenderer(GameWorld& world, ResourceManager& resourceManager) : OGLRenderer(*Window::GetWindow()), gameWorld(world) {
	glEnable(GL_DEPTH_TEST);

	// 加载阴影shader
	depthCubemapShader = (OGLShader*)resourceManager.LoadShader("JointShadowVert.glsl", "GameTechShadowFrag.glsl", "GameTechShadowGeom.glsl");
	depth2DShader = (OGLShader*)resourceManager.LoadShader("GameTechShadowVert.glsl", "GameTechShadowFrag.glsl");
	m_temp_shader = (OGLShader*)resourceManager.LoadShader("gameTechVert.glsl", "gameTechFrag.glsl");

	lightshader = (OGLShader*)resourceManager.LoadShader("GameTechVert.glsl", "GameTechFrag.glsl");
	jointshader = (OGLShader*)resourceManager.LoadShader("JointVert.glsl", "GameTechFrag.glsl");

	// 设置阴影
	//glGenTextures(1, &shadowTex);
	//glBindTexture(GL_TEXTURE_2D, shadowTex);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	//glBindTexture(GL_TEXTURE_2D, 0);

	//glGenFramebuffers(1, &shadowFBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, shadowTex, 0);
	//glDrawBuffer(GL_NONE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(1, 1, 1, 1);


	//setup light data
	std::vector<glm::vec3> pointLightPos;
	pointLightPos.push_back(glm::vec3(-154.0f, 30.0f, -15.0f));
	pointLightPos.push_back(glm::vec3(-80.0f, 30.0f, 0.0f));
	pointLightPos.push_back(glm::vec3(0.0f, 30.0f, 0.0f));

	//level 2 lights
	pointLightPos.push_back(glm::vec3(85.0f, 30.0f, -50.0f));
	pointLightPos.push_back(glm::vec3(169.0f, 30.0f, -27.0f));

	std::vector<glm::vec3> spotLightPos;
	spotLightPos.push_back(glm::vec3(10.0f, 30.0f, 30.0f));
	//spotLightPos.push_back(glm::vec3(10.0f, 30.0f, -30.0f));
	//spotLightPos.push_back(glm::vec3(30.0f, 30.0f, 0.0f));

	pointlight = new PointLight(pointLightPos);
	spotlight = new SpotLight(spotLightPos);

	//setup shadow map

	for (int i = 0; i < pointlight->getPointNumber(); ++i)
	{
		pointShadowMaps[i] = new Shadow();
	}

	for (int i = 0; i < spotlight->getSpotNumber(); ++i)
	{
		spotShadowMaps[i] = new Shadow();
	}
	//Skybox!
	skyboxShader = (OGLShader*)resourceManager.LoadShader("skyboxVertex.glsl", "skyboxFragment.glsl");
	skyboxMesh = new OGLMesh();
	skyboxMesh->SetVertexPositions({ Vector3(-1, 1,-1), Vector3(-1,-1,-1) , Vector3(1,-1,-1) , Vector3(1,1,-1) });
	skyboxMesh->SetVertexIndices({ 0,1,2,2,3,0 });
	skyboxMesh->UploadToGPU();

	LoadSkybox();
}

GameTechRenderer::~GameTechRenderer() {

	delete skyboxMesh;
	delete pointlight;
	delete spotlight;

	for (int i = 0; i < pointlight->getPointNumber(); ++i)
	{
		delete pointShadowMaps[i];
	}

	//glDeleteTextures(1, &shadowTex);
	//glDeleteFramebuffers(1, &shadowFBO);
}

void GameTechRenderer::LoadSkybox() {
	string filenames[6] = {
		"/nightskybox/Night_west.png",
		"/nightskybox/Night_east.png",
		"/nightskybox/Night_up.png",
		"/nightskybox/Night_down.png",
		"/nightskybox/Night_north.png",
		"/nightskybox/Night_south.png"
	//	"/Cubemap/skyrender0001.png",
	//	"/Cubemap/skyrender0003.png",
	//	"/Cubemap/skyrender0006.png",
	//	"/Cubemap/skyrender0002.png",
	//	"/Cubemap/skyrender0005.png"
	};

	int width[6] = { 0 };
	int height[6] = { 0 };
	int channels[6] = { 0 };
	int flags[6] = { 0 };

	vector<char*> texData(6, nullptr);

	for (int i = 0; i < 6; ++i) {
		TextureLoader::LoadTexture(filenames[i], texData[i], width[i], height[i], channels[i], flags[i]);
		if (i > 0 && (width[i] != width[0] || height[0] != height[0])) {
			std::cout << __FUNCTION__ << " cubemap input textures don't match in size?\n";
			return;
		}
	}
	glGenTextures(1, &skyboxTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	GLenum type = channels[0] == 4 ? GL_RGBA : GL_RGB;

	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width[i], height[i], 0, type, GL_UNSIGNED_BYTE, texData[i]);
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


void GameTechRenderer::RenderFrame() {
	BuildObjectList();
	SortObjectList();

	if (CameraComponent::GetMain() == nullptr)
		return;

	RenderShadowMap();// 渲染阴影
	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	RenderSkybox();		// 绘制天空盒

	RenderCamera();		// 摄像机

	glDisable(GL_CULL_FACE);
}

void GameTechRenderer::RenderLight()
{
	glUniform1f(glGetUniformLocation(boundShader->GetProgramID(), "material.shininess"), 32.0f);
	pointlight->render(boundShader);

	for (int i = 0; i < pointlight->getPointNumber(); ++i)
	{
		glActiveTexture(GL_TEXTURE4 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, pointShadowMaps[i]->getCubemapTexture());
		glUniform1i(glGetUniformLocation(boundShader->GetProgramID(), ("pointLights[" + std::to_string(i) + "].shadowMap").c_str()), 4 + i);
	}

	spotlight->render(boundShader);

	/*
	for (int i = 0; i < spotlight->getSpotNumber(); ++i)
	{
		glActiveTexture(GL_TEXTURE8 + i);
		glBindTexture(GL_TEXTURE_2D, spotShadowMaps[i]->get2DmapTexture());
		glUniform1i(glGetUniformLocation(lightshader->GetProgramID(), ("spotLight[" + std::to_string(i) + "].shadowMap").c_str()), 8 + i);
	}
	*/
}

void GameTechRenderer::BuildObjectList() {
	activeObjects.clear();

	gameWorld.OperateOnContents(
		[&](GameObject* o) {
			if (o->IsActive()) {
				const RenderObject* g = o->GetRenderObject();
				if (g) {
					activeObjects.emplace_back(g);
				}
			}
		}
	);
}

void GameTechRenderer::SortObjectList() {
	//std::sort(activeObjects.begin())

}

void GameTechRenderer::RenderShadowMap() {
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	BindShader(depthCubemapShader);

	int modelLocation = glGetUniformLocation(depthCubemapShader->GetProgramID(), "mvpMatrix");

	GLfloat aspect = 1.0;

	for (int li = 0; li < pointlight->getPointNumber(); ++li)
	{
		auto& shadowMap = pointShadowMaps[li];
		shadowMap->BindShadowFBOAsCubemap();
		glClearDepth(1.0);
		glClear(GL_DEPTH_BUFFER_BIT);

		shadowMap->DrawPointLightShadowMap(depthCubemapShader, *pointlight, li);

		for (const auto& i : activeObjects) {
			BindMesh((*i).GetMesh());

			if ((*i).GetAnimation()) {
				glUniform1i(glGetUniformLocation(depthCubemapShader->GetProgramID(), "hasJoints"), true);
				int jointLoc = glGetUniformLocation(boundShader->GetProgramID(), "joints");
				glUniformMatrix4fv(jointLoc, i->GetAnimation()->GetJointCount(), false, (float*)(i->GetFrameMatrices().data()));
			}
			else {
				glUniform1i(glGetUniformLocation(depthCubemapShader->GetProgramID(), "hasJoints"), false);
			}

			Matrix4 modelMatrix = (*i).GetTransform()->GetMatrix();
			glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

			int layerCount = (*i).GetMesh()->GetSubMeshCount();
			for (int i = 0; i < layerCount; ++i) {
				DrawBoundMesh(i);
			}
		}
	}

	/*

	BindShader(depth2DShader);

	modelLocation = glGetUniformLocation(depth2DShader->GetProgramID(), "mMatrix");
	int vpLocation = glGetUniformLocation(depth2DShader->GetProgramID(), "vpMatrix");
	//int mvpLocation = glGetUniformLocation(depth2DShader->GetProgramID(), "mvpMatrix");

	for (int li = 0; li < spotlight->getSpotNumber(); ++li)
	{
		auto lightPos = spotlight->getPos()[li];
		spotShadowMaps[li]->BindShadowFBOAs2D();
		glClearDepth(1.0);
		glClear(GL_DEPTH_BUFFER_BIT);

		spotShadowMaps[li]->DrawSpotLightShadowMap(depth2DShader, *spotlight, li);

		for (const auto& i : activeObjects) {
			BindMesh((*i).GetMesh());
			Matrix4 modelMatrix = i->GetTransform()->GetMatrix();
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, (float*)&(modelMatrix));
			glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(spotlight->getLightVPMatrix(li)));

			//Matrix4 modelMatrix = i->GetTransform()->GetMatrix();
			//Matrix4 mvpMatrix = Matrix4((float*)glm::value_ptr(spotlight->getLightVPMatrix(li)))* modelMatrix;
			//glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, (float*)& (mvpMatrix));
			int layerCount = (*i).GetMesh()->GetSubMeshCount();
			for (int i = 0; i < layerCount; ++i) {
				DrawBoundMesh(i);
			}
		}
	}
	*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GameTechRenderer::RenderSkybox() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, currentWidth, currentHeight);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = CameraComponent::GetMain()->GetCamera()->BuildViewMatrix();
	Matrix4 projMatrix = CameraComponent::GetMain()->GetCamera()->BuildProjectionMatrix(screenAspect);

	BindShader(skyboxShader);

	int projLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "projMatrix");
	int viewLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "viewMatrix");
	int texLocation = glGetUniformLocation(skyboxShader->GetProgramID(), "cubeTex");

	glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

	glUniform1i(texLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	BindMesh(skyboxMesh);
	DrawBoundMesh();

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void GameTechRenderer::RenderCamera() {
	glViewport(0, 0, currentWidth, currentHeight);
	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = CameraComponent::GetMain()->GetCamera()->BuildViewMatrix();
	Matrix4 projMatrix = CameraComponent::GetMain()->GetCamera()->BuildProjectionMatrix(screenAspect);

	//	OGLShader* activeShader = nullptr;
	int projLocation = 0;
	int viewLocation = 0;
	int modelLocation = 0;


	int colourLocation = 0;
	int hasVColLocation = 0;
//	int hasTexLocation = 0;
//	int shadowLocation = 0;

	//int lightPosLocation = 0;
	//int lightColourLocation = 0;
//	int lightRadiusLocation = 0;

	int cameraLocation = 0;

	//glActiveTexture(GL_TEXTURE0 + 1);
	//glBindTexture(GL_TEXTURE_2D, shadowTex);

	for (const auto& i : activeObjects) {

		if (i->GetAnimation()) {
			BindShader(jointshader);
		}
		else
			BindShader(lightshader);

		BindTextureToShader((OGLTexture*)(*i).GetDefaultTexture(), "mainTex", 0);
		projLocation = glGetUniformLocation(boundShader->GetProgramID(), "projMatrix");
		viewLocation = glGetUniformLocation(boundShader->GetProgramID(), "viewMatrix");
		modelLocation = glGetUniformLocation(boundShader->GetProgramID(), "modelMatrix");

		//	shadowLocation = glGetUniformLocation(lightshader->GetProgramID(), "shadowMatrix");
			colourLocation = glGetUniformLocation(boundShader->GetProgramID(), "objectColour");
			hasVColLocation = glGetUniformLocation(boundShader->GetProgramID(), "hasVertexColours");
		//	hasTexLocation = glGetUniformLocation(lightshader->GetProgramID(), "hasTexture");

		/*	lightPosLocation = glGetUniformLocation(lightshader->GetProgramID(), "lightPos");
			lightColourLocation = glGetUniformLocation(lightshader->GetProgramID(), "lightColour");
			lightRadiusLocation = glGetUniformLocation(lightshader->GetProgramID(), "lightRadius");*/

		cameraLocation = glGetUniformLocation(boundShader->GetProgramID(), "viewPos");
		Vector3 cameraPos = CameraComponent::GetMain()->GetCamera()->GetPosition();
		glUniform3fv(cameraLocation, 1, (float*)&cameraPos);

		glUniformMatrix4fv(projLocation, 1, false, (float*)&projMatrix);
		glUniformMatrix4fv(viewLocation, 1, false, (float*)&viewMatrix);

		//glUniform3fv(lightPosLocation, 1, (float*)&lightPosition);
		//glUniform4fv(lightColourLocation, 1, (float*)&lightColour);
		//glUniform1f(lightRadiusLocation, lightRadius);

	//	int shadowTexLocation = glGetUniformLocation(lightshader->GetProgramID(), "shadowTex");
	//	glUniform1i(shadowTexLocation, 1);


		Matrix4 modelMatrix = (*i).GetTransform()->GetMatrix();
		glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMatrix);

		//	Matrix4 fullShadowMat = shadowMatrix * modelMatrix;
		//	glUniformMatrix4fv(shadowLocation, 1, false, (float*)&fullShadowMat);

			glUniform4fv(colourLocation, 1, (float*)&i->GetColour());

			glUniform1i(hasVColLocation, !(*i).GetMesh()->GetColourData().empty());

		//	glUniform1i(hasTexLocation, (OGLTexture*)(*i).GetDefaultTexture() ? 1 : 0);

		RenderLight();

		BindMesh((*i).GetMesh());
		BindMaterial((*i).GetMaterial());

		if ((*i).GetAnimation()) {
			int jointLoc = glGetUniformLocation(boundShader->GetProgramID(), "joints");
			glUniformMatrix4fv(jointLoc, i->GetAnimation()->GetJointCount(), false, (float*)(i->GetFrameMatrices().data()));
		}

		int layerCount = (*i).GetMesh()->GetSubMeshCount();
		for (int i = 0; i < layerCount; ++i) {
			UpdateBoundMaterialLayer(i);
			DrawBoundMesh(i);
		}


	}
}

Matrix4 GameTechRenderer::SetupDebugLineMatrix()	const {
	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = CameraComponent::GetMain()->GetCamera()->BuildViewMatrix();// gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = CameraComponent::GetMain()->GetCamera()->BuildProjectionMatrix(screenAspect);// gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	return projMatrix * viewMatrix;
}

Matrix4 GameTechRenderer::SetupDebugStringMatrix()	const {
	return Matrix4::Orthographic(-1, 1.0f, 100, 0, 0, 100);
}
