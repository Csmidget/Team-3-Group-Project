#include "GameTechRenderer.h"
#include "../Engine/GameObject.h"
#include "../../Common/Camera.h"
#include "../../Common/Vector2.h"
#include "../../Common/Vector3.h"
#include "../../Common/TextureLoader.h"
#include "../../Common/Maths.h"


#include "../../include/glm/glm.hpp"
#include "../../include/glm/gtc/matrix_transform.hpp"
#include "../../include/glm/gtc/type_ptr.hpp"
#include "PointLight.h"
#include "SpotLight.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8508;



Matrix4 biasMatrix = Matrix4::Translation(Vector3(0.5, 0.5, 0.5)) * Matrix4::Scale(Vector3(0.5, 0.5, 0.5));



GameTechRenderer::GameTechRenderer(GameWorld& world, ResourceManager& resourceManager) : OGLRenderer(*Window::GetWindow()), gameWorld(world) {
	glEnable(GL_DEPTH_TEST);

	// 加载阴影shader
	depthShader = (OGLShader*)resourceManager.LoadShader("GameTechShadowVert.glsl", "GameTechShadowFrag.glsl", "GameTechShadowGeom.glsl");

	m_temp_shader = (OGLShader*)resourceManager.LoadShader("gameTechVert.glsl", "gameTechFrag.glsl");

	lightshader = (OGLShader*)resourceManager.LoadShader("GameTechVert.glsl", "GameTechFrag.glsl");

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


	//lightPosition = Vector3(0.0, 30.0f, -30.0f);




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

	//glDeleteTextures(1, &shadowTex);
	//glDeleteFramebuffers(1, &shadowFBO);
}

void GameTechRenderer::LoadSkybox() {
	string filenames[6] = {
		"/Cubemap/skyrender0004.png",
		"/Cubemap/skyrender0001.png",
		"/Cubemap/skyrender0003.png",
		"/Cubemap/skyrender0006.png",
		"/Cubemap/skyrender0002.png",
		"/Cubemap/skyrender0005.png"
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
	glEnable(GL_CULL_FACE);
	glClearColor(1, 1, 1, 1);
	BuildObjectList();
	SortObjectList();


	RenderShadowMap();// 渲染阴影
	RenderSkybox();		// 绘制天空盒
	RenderCamera();		// 摄像机

	glDisable(GL_CULL_FACE);
}

void GameTechRenderer::RenderLight()
{
	glUniform1f(glGetUniformLocation(lightshader->GetProgramID(), "material.shininess"), 32.0f);
	std::vector<glm::vec3> pointLightPos;
	pointLightPos.push_back(glm::vec3(-20.0f, 10.0f, -30.0f));


	std::vector<glm::vec3> spotLightPos;
	spotLightPos.push_back(glm::vec3(10.0f, 30.0f, 30.0f));
	spotLightPos.push_back(glm::vec3(10.0f, 30.0f, -30.0f));
	spotLightPos.push_back(glm::vec3(30.0f, 30.0f, 0.0f));

	PointLight pointlight(pointLightPos);
	SpotLight spotlight(spotLightPos);

	pointlight.render(lightshader);
	spotlight.render(lightshader);

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

	//GLfloat aspect = (GLfloat)SHADOWSIZE / (GLfloat)SHADOWSIZE;
	//Matrix4 shadowViewMatrix = Matrix4::BuildViewMatrix(lightPosition, Vector3(0, 0, 0), Vector3(0, 1, 0));
	//Matrix4 shadowProjMatrix = Matrix4::Perspective(100.0f, 500.0f, 1, 45.0f);

	//std::vector<Matrix4> shadowTransforms;
	//shadowTransforms.push_back(shadowProjMatrix * Matrix4::BuildViewMatrix(lightPosition, Vector3(1.0, 0.0, 0.0), Vector3(0.0, -1.0, 0.0)));
	//shadowTransforms.push_back(shadowProjMatrix * Matrix4::BuildViewMatrix(lightPosition, Vector3(-1.0, 0.0, 0.0), Vector3(0.0, -1.0, 0.0)));
	//shadowTransforms.push_back(shadowProjMatrix * Matrix4::BuildViewMatrix(lightPosition, Vector3(0.0, 1.0, 0.0), Vector3(0.0, 0.0, 1.0)));
	//shadowTransforms.push_back(shadowProjMatrix * Matrix4::BuildViewMatrix(lightPosition, Vector3(0.0, -1.0, 0.0), Vector3(0.0, 0.0, -1.0)));
	//shadowTransforms.push_back(shadowProjMatrix * Matrix4::BuildViewMatrix(lightPosition, Vector3(0.0, 0.0, 1.0), Vector3(0.0, -1.0, 0.0)));
	//shadowTransforms.push_back(shadowProjMatrix * Matrix4::BuildViewMatrix(lightPosition, Vector3(0.0, 0.0, -1.0), Vector3(0.0, -1.0, 0.0)));

	//BindShader(depthShader);
	//
	//glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	//glClear(GL_DEPTH_BUFFER_BIT);
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	//glCullFace(GL_FRONT);

	//int mvpLocation = glGetUniformLocation(depthShader->GetProgramID(), "mvpMatrix");

	//Matrix4 mvMatrix = shadowProjMatrix * shadowViewMatrix;

	//shadowMatrix = biasMatrix * mvMatrix; 

	//// 传入深度贴图
	//for (GLuint i = 0; i < 6; ++i)
	//{
	//	glUniformMatrix4fv(glGetUniformLocation(depthShader->GetProgramID(),
	//		("shadowMatrices[" + std::to_string(i) + "]").c_str()), 1, GL_FALSE,
	//		(float*)&(shadowTransforms[i]));
	//}

	//Vector3 lightpos = Vector3(0.0f, 30.0f, 0.0f);
	//glUniform1f(glGetUniformLocation(depthShader->GetProgramID(), "far_plane"), 25.0f);
	//glUniform3fv(glGetUniformLocation(depthShader->GetProgramID(), "lightPos"), 1, (float*)&lightpos);

	//// 渲染
	//for (const auto&i : activeObjects) {
	//	Matrix4 modelMatrix = (*i).GetTransform()->GetMatrix();
	//	glUniformMatrix4fv(glGetUniformLocation(depthShader->GetProgramID(), "model"), 1, false, (float*)&modelMatrix);
	//	BindMesh((*i).GetMesh());
	//	int layerCount = (*i).GetMesh()->GetSubMeshCount();
	//	for (int i = 0; i < layerCount; ++i) {
	//		DrawBoundMesh(i);
	//	}
	//}

	//glViewport(0, 0, currentWidth, currentHeight);
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glCullFace(GL_BACK);
}

void GameTechRenderer::RenderSkybox() {
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

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
	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	//	OGLShader* activeShader = nullptr;
	int projLocation = 0;
	int viewLocation = 0;
	int modelLocation = 0;


	//int colourLocation = 0;
//	int hasVColLocation = 0;
//	int hasTexLocation = 0;
//	int shadowLocation = 0;

	//int lightPosLocation = 0;
	//int lightColourLocation = 0;
//	int lightRadiusLocation = 0;

	int cameraLocation = 0;

	//glActiveTexture(GL_TEXTURE0 + 1);
	//glBindTexture(GL_TEXTURE_2D, shadowTex);

	glUseProgram(lightshader->GetProgramID());

	for (const auto& i : activeObjects) {

		BindTextureToShader((OGLTexture*)(*i).GetDefaultTexture(), "mainTex", 0);
		projLocation = glGetUniformLocation(lightshader->GetProgramID(), "projMatrix");
		viewLocation = glGetUniformLocation(lightshader->GetProgramID(), "viewMatrix");
		modelLocation = glGetUniformLocation(lightshader->GetProgramID(), "modelMatrix");

		//	shadowLocation = glGetUniformLocation(lightshader->GetProgramID(), "shadowMatrix");
		//	colourLocation = glGetUniformLocation(lightshader->GetProgramID(), "objectColour");
		//	hasVColLocation = glGetUniformLocation(lightshader->GetProgramID(), "hasVertexColours");
		//	hasTexLocation = glGetUniformLocation(lightshader->GetProgramID(), "hasTexture");

		/*	lightPosLocation = glGetUniformLocation(lightshader->GetProgramID(), "lightPos");
			lightColourLocation = glGetUniformLocation(lightshader->GetProgramID(), "lightColour");
			lightRadiusLocation = glGetUniformLocation(lightshader->GetProgramID(), "lightRadius");*/

		cameraLocation = glGetUniformLocation(lightshader->GetProgramID(), "viewPos");
		glUniform3fv(cameraLocation, 1, (float*)&gameWorld.GetMainCamera()->GetPosition());

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

		//	glUniform4fv(colourLocation, 1, (float*)&i->GetColour());

		//	glUniform1i(hasVColLocation, !(*i).GetMesh()->GetColourData().empty());

		//	glUniform1i(hasTexLocation, (OGLTexture*)(*i).GetDefaultTexture() ? 1 : 0);

		RenderLight();

		BindMesh((*i).GetMesh());
		BindMaterial((*i).GetMaterial());
		int layerCount = (*i).GetMesh()->GetSubMeshCount();
		for (int i = 0; i < layerCount; ++i) {
			UpdateBoundMaterialLayer(i);
			DrawBoundMesh(i);
		}


	}
}

Matrix4 GameTechRenderer::SetupDebugLineMatrix()	const {
	float screenAspect = (float)currentWidth / (float)currentHeight;
	Matrix4 viewMatrix = gameWorld.GetMainCamera()->BuildViewMatrix();
	Matrix4 projMatrix = gameWorld.GetMainCamera()->BuildProjectionMatrix(screenAspect);

	return projMatrix * viewMatrix;
}

Matrix4 GameTechRenderer::SetupDebugStringMatrix()	const {
	return Matrix4::Orthographic(-1, 1.0f, 100, 0, 0, 100);
}
