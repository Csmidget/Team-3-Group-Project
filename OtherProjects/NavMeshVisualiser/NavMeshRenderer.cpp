#include "NavMeshRenderer.h"
#include "../../Common/Assets.h"
#include "../../Common/Camera.h"

#include "../../Common/Vector3.h"
#include "../../Common/Matrix4.h"
#include "../../Common/MeshAnimation.h"

#include <fstream>
#include <iostream>
using namespace NCL;

NavMeshRenderer::NavMeshRenderer() : OGLRenderer(*Window::GetWindow())	{
	navMesh = new OGLMesh();

	std::ifstream mapFile(Assets::DATADIR + "simple.navmesh");

	int vCount = 0;
	int iCount = 0;

	mapFile >> vCount;
	mapFile >> iCount;

	vector<Vector3>			meshVerts;
	vector<unsigned int>	meshIndices;

	for (int i = 0; i < vCount; ++i) {
		Vector3 temp;
		mapFile >> temp.x;
		mapFile >> temp.y;
		mapFile >> temp.z;
		meshVerts.emplace_back(temp);
	}

	for (int i = 0; i < iCount; ++i) {
		unsigned int temp = -1;
		mapFile >> temp;
		meshIndices.emplace_back(temp);
	}

	struct TriNeighbours {
		int indices[3];
	};

	int numTris = iCount / 3;	//the indices describe n / 3 triangles
	vector< TriNeighbours> allNeighbours;
	//Each of these triangles will be sharing edges with some other triangles
	//so it has a maximum of 3 'neighbours', desribed by an index into n / 3 tris
	//if its a -1, then the edge is along the edge of the map...
	for (int i = 0; i < numTris; ++i) {
		TriNeighbours neighbours;
		mapFile >> neighbours.indices[0];
		mapFile >> neighbours.indices[1];
		mapFile >> neighbours.indices[2];
		allNeighbours.emplace_back(neighbours);
	}

	navMesh->SetVertexPositions(meshVerts);
	navMesh->SetVertexIndices(meshIndices);
	navMesh->UploadToGPU();

	navShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	camera = new Camera();
	camera->SetNearPlane(1.0f);
	camera->SetFarPlane(1000.0f);
	camera->SetPosition(Vector3(0, 3, 10));
}

NavMeshRenderer::~NavMeshRenderer() {
	delete navMesh;
	delete navShader;
	delete camera;

}

void NavMeshRenderer::Update(float dt) {
	camera->UpdateCamera(dt);
}

void NavMeshRenderer::RenderFrame() {
	BindShader(navShader);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	float screenAspect = (float)currentWidth / (float)currentHeight;

	Matrix4 viewMatrix = camera->BuildViewMatrix();
	Matrix4 projMatrix = camera->BuildProjectionMatrix(screenAspect);
	Matrix4 modelMat = Matrix4();

	int projLocation	= glGetUniformLocation(navShader->GetProgramID(), "projMatrix");
	int viewLocation	= glGetUniformLocation(navShader->GetProgramID(), "viewMatrix");
	int modelLocation	= glGetUniformLocation(navShader->GetProgramID(), "modelMatrix");
	int hasTexLocation	= glGetUniformLocation(navShader->GetProgramID(), "hasTexture");

	glUniformMatrix4fv(modelLocation, 1, false, (float*)&modelMat);
	glUniformMatrix4fv(viewLocation , 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(projLocation , 1, false, (float*)&projMatrix);

	glUniform1i(hasTexLocation, 0);

	BindMesh(navMesh);
	DrawBoundMesh();
}