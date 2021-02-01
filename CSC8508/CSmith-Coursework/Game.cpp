#include "Game.h"

#include "PlayerObject.h"
#include "MainMenuState.h"
#include "Checkpoint.h"
#include "ObjectFactory.h"
#include "GameTechRenderer.h"

#include "../Engine/PushdownMachine.h"
#include "../Engine/PhysicsSystem.h"
#include "../Engine/NavigationMap.h"


using namespace NCL;
using namespace CSC8508;


// ###!! Init functions can be found under the Game Factory Functions filter !!###

Game::Game() {
	world		= new GameWorld();
	renderer	= new GameTechRenderer(*world);
	physics		= new PhysicsSystem(*world);
	objectFactory = new ObjectFactory();
	goal = nullptr;

	useGravity		= true;
	physics->UseGravity(true);

	pause = false;
	quit = false;

	gameStateMachine = new PushdownMachine(new MainMenuState(this));
		
	Debug::SetRenderer(renderer);

	InitCamera();
}

Game::~Game()	{
	Clear();

	delete physics;
	delete renderer;
	delete world;
	delete objectFactory;
	delete gameStateMachine;
}

void Game::UpdateGame(float dt) {

	UpdateFramerateDisplay(dt);
	UpdateKeys();

	if (!pause) {
		physics->Update(dt);
		world->UpdateWorld(dt);
	}

	Vector3 playerBroadphase;
	if (player && player->GetBroadphaseAABB(playerBroadphase)) {
		for (auto cp : checkpoints) {
			if (CollisionDetection::AABBTest(player->GetTransform().GetPosition(), cp->GetPosition(), playerBroadphase, cp->GetHalfDims())) {
				//player is inside checkpoint area
				if (!player->GetCheckpoint() || cp->GetPriority() >= player->GetCheckpoint()->GetPriority())
					player->SetCheckpoint(cp);
			}
		}
	}

	if (!gameStateMachine->Update(dt) || Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE)) {
		quit = true;
		return;
	}

	renderer->Update(dt);
	Debug::FlushRenderables(dt);
	renderer->Render();
}

void Game::SetUseGravity(bool val) {
	useGravity = val;
	physics->UseGravity(val);
}

void Game::UpdateKeys() {

	//Running certain physics updates in a consistent order might cause some
	//bias in the calculations - the same objects might keep 'winning' the constraint
	//allowing the other one to stretch too much etc. Shuffling the order so that it
	//is random every frame can help reduce such bias.
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F9)) {
		world->ShuffleConstraints(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F10)) {
		world->ShuffleConstraints(false);
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F7)) {
		world->ShuffleObjects(true);
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F8)) {
		world->ShuffleObjects(false);
	}

}

void Game::UpdateFramerateDisplay(float dt) {
	frameTimes.push_back(dt);

	if (frameTimes.size() > 500) {
		frameTimes.erase(frameTimes.begin());
	}

	float frameRate{ 0 };

	for (auto frameTime : frameTimes) {
		frameRate += frameTime;
	}

	frameRate /= frameTimes.size();
	frameRate = 1 / frameRate;

	Debug::Print(std::to_string((int)frameRate), Vector2(95, 2), Debug::YELLOW, 1.2f);
}

void Game::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));
}

void Game::Clear() {
	world->ClearAndErase();

	physics->Clear();
	useGravity = true; 
	physics->SetGravity(Vector3(0.0f, -9.8f, 0.0f));

	//As this is a game object it will be deleted by  world ClearAndErase()
	player = nullptr;

	delete navMap;
	navMap = nullptr;
	
	for (auto cp : checkpoints) {
		delete cp;
	}
	checkpoints.clear();

	//Opponent objects will be deleted by the world clearanderase.
	opponents.clear();

}

NavigationPath Game::GetPath(Vector3 start, Vector3 end, float maxCost) const {

	NavigationPath generatedPath;

	if (!navMap) {
		generatedPath.PushWaypoint(end);
		
	}
	else {
		navMap->FindPath(start, end, generatedPath, maxCost);
	}

	return generatedPath;
}
