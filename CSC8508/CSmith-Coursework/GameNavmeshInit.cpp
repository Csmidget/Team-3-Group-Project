#include "Game.h"
#include "ObjectFactory.h"
#include "../Engine/NavigationMesh.h"
#include "Checkpoint.h"
#include "AIObject.h"
#include "../../Common/Camera.h"

void Game::InitNavmeshWorld() {
	Clear();

	world->GetMainCamera()->SetPosition(Vector3(30, 25, 60));
	world->GetMainCamera()->SetPitch(-30.0f);
	world->GetMainCamera()->SetYaw(0.0f);

	InitNavmeshLvlBaseGeometry();
	InitNavmeshLvlKillPlanes();
	InitNavmeshLvlPlayers(1);

	navMap = new NavigationMesh("simple.navmesh");

}

void Game::InitNavmeshLvlBaseGeometry() {

	//Main section
	objectFactory->CreateFloor(world, Vector3(0, 0, 0), Vector2(40, 40));

	objectFactory->CreateAABBCube(world, Vector3( 26, 2, -13), Vector3(14, 2, 27), 0.0f, true);
	objectFactory->CreateAABBCube(world, Vector3(-10, 2, -5), Vector3(2, 2, 15), 0.0f, true);
	objectFactory->CreateAABBCube(world, Vector3(0, 2, 12), Vector3(12, 2, 2), 0.0f, true);
	objectFactory->CreateOBBCube(world, Vector3(26, 0.25, 17.35f),Matrix4::Rotation(25,Vector3(1,0,0)), Vector3(12, 2, 4.61f), 0.0f,false,true);

	objectFactory->CreateScoreBonus(world, Vector3(-37, 1, 37), 30.0f);
	objectFactory->CreateScoreBonus(world, Vector3(-9.5, 5, 11.5), 30.0f);

	goal = new Checkpoint(Vector3(26.5f, 4, -34.61f), Vector3(5, 5, 5), 1);

	//Side Section
	objectFactory->CreateFloor(world, Vector3(48, 0, 30), Vector2(8, 10));
	objectFactory->CreateFloor(world, Vector3(71, 0, 0), Vector2(15, 40));

	objectFactory->CreateOBBCube(world, Vector3(67.5, 2, -27.5), Matrix3::Rotation(44, Vector3(0, 1, 0)), Vector3(2, 2, 6), 0.0f, false, true);
	objectFactory->CreateOBBCube(world, Vector3(71, 2, -12), Matrix3::Rotation(-90, Vector3(0, 1, 0)), Vector3(2, 2, 6), 0.0f, false, true);
	objectFactory->CreateOBBCube(world, Vector3(61.5, 2, 3.5), Matrix3::Rotation(-56, Vector3(0, 1, 0)), Vector3(2, 2, 6), 0.0f, false, true);
	objectFactory->CreateOBBCube(world, Vector3(80, 2, 2), Matrix3::Rotation(-127, Vector3(0, 1, 0)), Vector3(2, 2, 6), 0.0f, false, true);
	objectFactory->CreateOBBCube(world, Vector3(73, 2, 12.5), Matrix3::Rotation(-90, Vector3(0, 1, 0)), Vector3(2, 2, 6), 0.0f, false, true);
	objectFactory->CreateOBBCube(world, Vector3(48, 2, 26),Quaternion(), Vector3(2, 2, 6), 0.0f, false, true);

	objectFactory->CreateScoreBonus(world, Vector3(59, 1, -1), 20.0f);
}

void Game::InitNavmeshLvlKillPlanes() {
	world->AddKillPlane(new Plane(Vector3(0, 1, 0), Vector3(0, -5, 0)));
}

void Game::InitNavmeshLvlPlayers(int opponentCount) {
	opponents.push_back(objectFactory->CreateRaceAI(this, Vector3(0, 2, 0), "AI 1", 40.0f, 40.0f));
	opponents.push_back(objectFactory->CreateRaceAI(this, Vector3(83, 2, -36), "AI 2",40.0f,40.0f));
	opponents[0]->SetSpeed(3.0f);
	opponents[1]->SetSpeed(3.0f);
}