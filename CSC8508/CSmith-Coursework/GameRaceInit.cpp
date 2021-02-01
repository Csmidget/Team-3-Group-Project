#include "Game.h"
#include "Checkpoint.h"
#include "ObjectFactory.h"
#include "../Engine/NavigationGrid.h"


void Game::InitRaceWorld(int players) {
	Clear();

	InitRaceBaseGeometry();
	InitRaceKillPlanes();
	InitRaceCheckpoints();
	InitRacePlayers(players);

	//navGrid = new NavigationGrid("RaceGrid.txt", Vector3(125, 0, 125));
	navMap = new NavigationGrid(world->GetStaticObjectTree(), Vector3(0, 0, 0), 1000, Vector2(200, 200), 2.5f);
}

void Game::InitRaceBaseGeometry() {

	//Starting zone
	objectFactory->CreateFloor(world, Vector3(-100, -0.5f, 100), Vector2(20, 20));

	//First Gauntlet
	{
		//Floor
		objectFactory->CreateFloor(world, Vector3(-100, -0.5f, 0.0f), Vector2(15, 80));

		//First section
		{
			//Front wall
			objectFactory->CreateAABBCube(world, Vector3(-100, 2, 80), Vector3(7.5f, 2, 1.25f), 0.0f, true);
			objectFactory->CreateAABBCube(world, Vector3(-85, 2, 80), Vector3(2.5f, 2, 1.25f), 0.0f, true);
			objectFactory->CreateAABBCube(world, Vector3(-115, 2, 80), Vector3(2.5f, 2, 1.25f), 0.0f, true);

			//Coin room Walls
			objectFactory->CreateAABBCube(world, Vector3(-106.25, 2, 72.5), Vector3(1.25f, 2, 6.25f), 0.0f, true);
			objectFactory->CreateAABBCube(world, Vector3(-93.75, 2, 72.5), Vector3(1.25f, 2, 6.25f), 0.0f, true);
			objectFactory->CreateAABBCube(world, Vector3(-95, 2, 65.0f), Vector3(2.5f, 2, 1.25f), 0.0f, true);
			objectFactory->CreateAABBCube(world, Vector3(-105, 2, 65.0f), Vector3(2.5f, 2, 1.25f), 0.0f, true);

			//Coin room coins
			objectFactory->CreateScoreBonus(world, Vector3(-103.0f, 1.0f, 68.75f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-100.0f, 1.0f, 68.75f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-97.0f, 1.0f, 68.75f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-103.0f, 1.0f, 76.25f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-100.0f, 1.0f, 76.25f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-97.0f, 1.0f, 76.25f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-103.0f, 1.0f, 72.5f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-100.0f, 1.0f, 72.5f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-97.0f, 1.0f, 72.5f), 30.0f);

			//Coin room Tunnel
			objectFactory->CreateAABBCube(world, Vector3(-103.75, 2, 57.5f), Vector3(1.25f, 2, 6.25f), 0.0f, true);
			objectFactory->CreateAABBCube(world, Vector3(-96.25, 2, 57.5f), Vector3(1.25f, 2, 6.25f), 0.0f, true);

			objectFactory->CreateScoreBonus(world, Vector3(-113.0f, 1.0f, 43.0f), 60.0f);
			objectFactory->CreateScoreBonus(world, Vector3( -87.0f, 1.0f, 43.0f), 60.0f);

			//Back wall
			objectFactory->CreateAABBCube(world, Vector3(-108.75, 2, 40), Vector3(6.25f, 2, 1.25f), 0.0f, true);
			objectFactory->CreateAABBCube(world, Vector3(-91.25, 2, 40), Vector3(6.25, 2, 1.25f), 0.0f, true);
		}

		//Swamp Section
		{
			//Central swamp
			objectFactory->CreateSwampFloor(world, Vector3(-100, -0.499f, 20.0f), Quaternion(), Vector2(10, 10));
			objectFactory->CreateSwampFloor(world, Vector3(-112.51f, -0.499f, 20.0f), Quaternion(), Vector2(2.51f, 2.5f));
			objectFactory->CreateSwampFloor(world, Vector3(-87.49f, -0.499f, 20.0f), Quaternion(), Vector2(2.51f, 1.25f));

			//Coins
			objectFactory->CreateScoreBonus(world, Vector3(-105.0f, 1.0f, 15.0f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-105.0f, 1.0f, 20.0f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-105.0f, 1.0f, 25.0f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-100.0f, 1.0f, 15.0f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-100.0f, 1.0f, 20.0f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-100.0f, 1.0f, 25.0f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-95.0f, 1.0f, 15.0f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-95.0f, 1.0f, 20.0f), 30.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-95.0f, 1.0f, 25.0f), 30.0f);

			//Back wall
			objectFactory->CreateAABBCube(world, Vector3(-108.75, 1, 0), Vector3(6.25f, 1, 1.25f), 0.0f, true);
			objectFactory->CreateAABBCube(world, Vector3(-91.25, 1, 0), Vector3(6.25, 1, 1.25f), 0.0f, true);
		}

		//Connector
		{
			//Left Coin
			objectFactory->CreateScoreBonus(world, Vector3(-112.5f, 1.0f, -3.75f), 30.0f);

			//Horizontal wall
			objectFactory->CreateAABBCube(world, Vector3(-103.75, 1, -7.5f), Vector3(11.25, 1, 1.25f), 0.0f, true);
		}

		//Spinning block
		{
			objectFactory->CreateSpinningBlock(world, Vector3(-100, 2.5f, -30), Vector3(0, 1, 0), -12500.0f, 15.0f);
			objectFactory->CreatePathBlocker(world, Vector2(-95.0f, -25.0f), Vector2(10.0f, 10.0f));
		}

		//Ending horse-shoe structure
		{
			objectFactory->CreateAABBCube(world, Vector3(-100, 2, -57.5), Vector3(6.25, 2, 1.25), 0.0f, true);
			objectFactory->CreateAABBCube(world, Vector3(-105, 2, -65), Vector3(1.25, 2, 6.25), 0.0f, true);
			objectFactory->CreateAABBCube(world, Vector3(-95, 2, -65), Vector3(1.25, 2, 6.25), 0.0f, true);

			objectFactory->CreateScoreBonus(world, Vector3(-100, 1, -62), 60.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-100, 1, -65), 60.0f);
			objectFactory->CreateScoreBonus(world, Vector3(-100, 1, -68), 60.0f);
		}
	}
	//Checkpoint1
	objectFactory->CreateFloor(world, Vector3(-100, -0.5f, -100.0f), Vector2(20, 20));
}

void Game::InitRaceKillPlanes() {
	//Kill plane below the floor
	world->AddKillPlane(new Plane(Vector3(0, 1, 0), Vector3(0, -5, 0)));
}


void Game::InitRaceCheckpoints() {
	goal = new Checkpoint(Vector3(-100, 10, -100), Vector3(20, 10, 20), 1);

}

void Game::InitRacePlayers(int opponentCount) {
	player = objectFactory->CreatePlayer(this, Vector3(-100, 5, 95));

	Vector3 firstSpawn(-110, 5, 100);

	//The first few opponents are predetermined.
	if (opponentCount >= 1)
		opponents.push_back(objectFactory->CreateRaceAI(this, Vector3(-110, 5, 100), "Enemy 1"));
	if (opponentCount >= 2)
		opponents.push_back(objectFactory->CreateRaceAI(this, Vector3(-105, 5, 100), "Enemy 2", 15.0f, 50.0f));
	if (opponentCount >= 3)
		opponents.push_back(objectFactory->CreateRaceAI(this, Vector3(-100, 5, 100), "Enemy 3", 20.0f, 70.0f));
	if (opponentCount >= 4)
		opponents.push_back(objectFactory->CreateRaceAI(this, Vector3(-95, 5, 100), "Enemy 4", 70.0f, 100.0f));
	if (opponentCount >= 5)
		opponents.push_back(objectFactory->CreateRaceAI(this, Vector3(-90, 5, 100), "Enemy 5",50.0f , 100.0f, 0.0f, 3.0f));
	if (opponentCount >= 6)
		opponents.push_back(objectFactory->CreateRaceAI(this, Vector3(-110, 5, 103), "Enemy 6", 60.0f, 100.0f, 0.0f, 3.0f));

	for (int i = 7; i <= opponentCount; ++i) {

		int x = (i - 1) % 5;
		int y = (i - 1) / 5;
		Vector3 spawnPos = firstSpawn + Vector3(x * 5.0f, 0, y * 5.0f);

		//1-30
		float coinHuntRange = (float)(rand() % 30) + 1;
		//cointHuntRange-150
		float maxCoinDistance = (float)(rand() % 151 - coinHuntRange) + coinHuntRange;
		//0-10
		float angerThreshold = (float)(rand() % 11);
		//1-4
		float strength = 1 + (float)(rand() % 30) / 10.0f;

		opponents.push_back(objectFactory->CreateRaceAI(this, spawnPos, "Enemy " + std::to_string(i), coinHuntRange, maxCoinDistance, angerThreshold, strength));
	}
}