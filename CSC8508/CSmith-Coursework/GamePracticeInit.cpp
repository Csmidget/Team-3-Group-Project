#include "Game.h"
#include "Checkpoint.h"
#include "ObjectFactory.h"

void Game::InitPracticeWorld() {
	Clear();

	InitPracticeKillPlanes();
	InitPracticeBaseGeometry();
	InitPracticeGauntlet1();
	InitPracticeSlope();
	InitPracticeGauntlet2();
	InitPracticePlayers();
	InitPracticeCheckpoints();
}

void Game::InitPracticeKillPlanes() {
	//Kill plane below the floor
	world->AddKillPlane(new Plane(Vector3(0, 1, 0), Vector3(0, -5, 0)));
}

void Game::InitPracticeBaseGeometry() {

	//Starting zone
	objectFactory->CreateFloor(world, Vector3(-100, -0.5f, 100), Vector2(20, 20));

	//First Gauntlet
	objectFactory->CreateFloor(world, Vector3(-100, -0.5f, 0.0f), Vector2(10, 80));

	//Checkpoint1
	objectFactory->CreateFloor(world, Vector3(-100, -0.5f, -100.0f), Vector2(20, 20));

	//Slope
	objectFactory->CreateOrientedFloor(world, Vector3(0.0f, 50.5f, -100.0f), Quaternion::EulerAnglesToQuaternion(0, 0, 32.5), Vector2(95.15f, 10));

	//Checkpoint2
	objectFactory->CreateFloor(world, Vector3(100.0f, 101.5f, -100.0f), Vector2(20, 20));

	//Goal
	objectFactory->CreateFloor(world, Vector3(100.0f, 101.5f, 100.0f), Vector2(20, 20));

}

void Game::InitPracticeGauntlet1() {

	objectFactory->CreateSpinningBlock(world, Vector3(-100, 3.0f, -30.0f), Vector3(0, 1, 0), -10000.0f);
	objectFactory->CreateSpinningBlock(world, Vector3(-100, 3.0f, 0.0f), Vector3(0, 1, 0), 10000.0f);
	objectFactory->CreateSpinningBlock(world, Vector3(-100, 3.0f, 30.0f), Vector3(0, 1, 0), -10000.0f);

	//Coins
	objectFactory->CreateScoreBonus(world, Vector3(-100, 1.0f, 60.0f));
	objectFactory->CreateScoreBonus(world, Vector3(-100, 1.0f, 45.0f));
	objectFactory->CreateScoreBonus(world, Vector3(-105, 1.0f, 30.0f));
	objectFactory->CreateScoreBonus(world, Vector3(-100, 1.0f, 15.0f));
	objectFactory->CreateScoreBonus(world, Vector3(-95, 1.0f, 0.0f));
	objectFactory->CreateScoreBonus(world, Vector3(-100, 1.0f, -15.0f));
	objectFactory->CreateScoreBonus(world, Vector3(-105, 1.0f, -30.0f));
	objectFactory->CreateScoreBonus(world, Vector3(-100, 1.0f, -45.0f));
	objectFactory->CreateScoreBonus(world, Vector3(-100, 1.0f, -60.0f));
}

void Game::InitPracticeSlope() {

	//Falling objects
	for (int i = 1; i <= 4; ++i) {
		float height = i * 150.0f;

		objectFactory->CreateCapsule(world, Vector3(70.0f, height, -105.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true);
		objectFactory->CreateCapsule(world, Vector3(70.0f, height, -100.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true);
		objectFactory->CreateCapsule(world, Vector3(70.0f, height, -95.0f), Quaternion(), 1.0f, 0.5f, 0.3f, true);

		objectFactory->CreateSphere(world, Vector3(70.0f, height + 50, -105.0f), 1.0f, 0.2f, true);
		objectFactory->CreateSphere(world, Vector3(70.0f, height + 50, -100.0f), 1.0f, 0.2f, true);
		objectFactory->CreateSphere(world, Vector3(70.0f, height + 50, -95.0f), 1.0f, 0.2f, true);

		objectFactory->CreateOBBCube(world, Vector3(70.0f, height + 100, -107.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true);
		objectFactory->CreateOBBCube(world, Vector3(70.0f, height + 100, -100.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true);
		objectFactory->CreateOBBCube(world, Vector3(70.0f, height + 100, -93.0f), Quaternion(), Vector3(1, 1, 1), 0.1f, true);
	}

	//Coins
	objectFactory->CreateScoreBonus(world, Vector3(-75.0f, 4.5f, -100.0f));
	objectFactory->CreateScoreBonus(world, Vector3(-50.0f, 20.5f, -100.0f));
	objectFactory->CreateScoreBonus(world, Vector3(-25.0f, 36.25f, -100.0f));
	objectFactory->CreateScoreBonus(world, Vector3(0.0f, 52.0f, -100.0f));
	objectFactory->CreateScoreBonus(world, Vector3(25.0f, 67.75, -100.0f));
	objectFactory->CreateScoreBonus(world, Vector3(50.0f, 83.5f, -100.0f));
	objectFactory->CreateScoreBonus(world, Vector3(75.0f, 99.5f, -100.0f));
}

void Game::InitPracticeGauntlet2() {
	float y = 101.5f;


	//Checkpoint decorations
	objectFactory->CreateBouncePad(world, Vector3(115.0f, y + 1.3f, -115.0f), Matrix4::Rotation(-45, Vector3(0, 1, 0)) * Matrix4::Rotation(45, Vector3(1, 0, 0)), 5.85f, Vector2(4.95f, 2.25f));
	objectFactory->CreateBouncePad(world, Vector3(85.0f, y + 1.3f, -85.0f), Matrix4::Rotation(-45, Vector3(0, 1, 0)) * Matrix4::Rotation(-45, Vector3(1, 0, 0)), 5.85f, Vector2(4.95f, 2.25f));

	objectFactory->CreateBouncePad(world, Vector3(85.0f, y + 0.5f, -115.0f),Quaternion(), 5.85f, Vector2(4.5f, 4.5f));
	objectFactory->CreateSphere(world, Vector3(85.0f, y + 10.0f, -115.0f), 1.0f, 4.0f, true);

	//Gauntlet
	objectFactory->CreateSlipperyFloor(world, Vector3(115.0f, y, -60.0f), Quaternion(), Vector2(5.0f, 20.0f));
	objectFactory->CreateBouncePad(world, Vector3(115.0f, y - 1.1f, -38.25f), Matrix4::Rotation(30, Vector3(1, 0, 0)), 5.3f, Vector2(4.95f, 2.25f));

	objectFactory->CreateSlipperyFloor(world, Vector3(100.0f, y, -45.0f), Quaternion(), Vector2(10.0f, 1.5f));

	objectFactory->CreateTreadmill(world, Vector3(85.0f, y, -25.0f), Quaternion(), 4.0f, Vector2(5.0f, 25.0f));

	objectFactory->CreateSlipperyFloor(world, Vector3(103.0f, y, -5.0f), Quaternion(), Vector2(13.0f, 1.0f));

	objectFactory->CreateSlipperyFloor(world, Vector3(115.5f, y, 3.0f), Quaternion(), Vector2(0.5f, 7.0f));

	objectFactory->CreateTreadmill(world, Vector3(105.0f, y, 15.0f), Matrix4::Rotation(90, Vector3(0, 1, 0)), 4.0f, Vector2(5.0f, 15.0f));
	objectFactory->CreateTreadmill(world, Vector3(85.0f, y, 25.0f), Matrix4::Rotation(180, Vector3(0, 1, 0)), 4.0f, Vector2(5.0f, 15.0f));
	objectFactory->CreateTreadmill(world, Vector3(95.0f, y, 45.0f), Matrix4::Rotation(-90, Vector3(0, 1, 0)), 4.0f, Vector2(5.0f, 15.0f));
	objectFactory->CreateTreadmill(world, Vector3(115.0f, y, 35.0f), Quaternion(), 4.0f, Vector2(5.0f, 15.0f));

	objectFactory->CreateSphere(world, Vector3(115, y + 2, 35.0f), 1.0f, 0.6f, true);

	objectFactory->CreateSlipperyFloor(world, Vector3(100.0f, y, 65.0f), Quaternion(), Vector2(2.0f, 15.0f));

	float coinY = y + 1.5f;
	//Coins
	objectFactory->CreateScoreBonus(world, Vector3(87.5f, coinY, -49.5f));
	objectFactory->CreateScoreBonus(world, Vector3(82.5f, coinY, -49.5f));

	objectFactory->CreateScoreBonus(world, Vector3(85.0f, coinY, -35.0f));
	objectFactory->CreateScoreBonus(world, Vector3(85.0f, coinY, -25.0f));
	objectFactory->CreateScoreBonus(world, Vector3(85.0f, coinY, -15.0f));

	objectFactory->CreateScoreBonus(world, Vector3(115.5f, y + 7.0f, -25.0f));

	objectFactory->CreateScoreBonus(world, Vector3(105.0f, coinY, 15.0f));
	objectFactory->CreateScoreBonus(world, Vector3(85.0f, coinY, 25.0f));
	objectFactory->CreateScoreBonus(world, Vector3(95.0f, coinY, 45.0f));
	objectFactory->CreateScoreBonus(world, Vector3(115.0f, coinY, 35.0f));
}

void Game::InitPracticePlayers() {
	player = objectFactory->CreatePlayer(this, Vector3(-100, 5, 100));
}

void Game::InitPracticeCheckpoints() {
	checkpoints.push_back(new Checkpoint(Vector3(-100, 10, -100), Vector3(20, 10, 20), 1));
	checkpoints.push_back(new Checkpoint(Vector3(100, 111, -100), Vector3(20, 10, 20), 2));

	goal = new Checkpoint(Vector3(100, 111, 100), Vector3(20, 10, 20), 2);
}
