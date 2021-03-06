#include "Game.h"
#include "JSONLevelFactory.h"
#include "GameTechRenderer.h"
#include "IntroState.h"
#include "CameraComponent.h"
#include "LocalNetworkPlayerComponent.h"
#include "PlayerRayFeetComponent.h"

#include "../Engine/GameWorld.h"
#include "../Engine/Physics/PhysicsEngine/BulletWorld.h"
#include "../Engine/NetworkManager.h"
#include "../Engine/PushdownMachine.h"

#include"../Audio/SoundManager.h"
#include"../Audio/SoundInstance.h"

#include "../../Plugins/OpenGLRendering/OGLResourceManager.h"


using namespace NCL;
using namespace CSC8508;
using namespace Maths;

Game::Game() {
	resourceManager = new OGLResourceManager();
	world = new GameWorld();
	renderer = new GameTechRenderer(*world, *resourceManager);
	physics		= new physics::BulletWorld();
	gameStateMachine = new PushdownMachine(new IntroState(this));
	//networkManager = new NetworkManager();

	forceMagnitude = 10.0f;
	useGravity = false;
	inSelectionMode = false;	

	Debug::SetRenderer(renderer);
	Audio::SoundManager::Init();
	InitialiseAssets();

	//Play Background Music
	music = new Audio::SoundInstance();
	music->SetVolume(0.2f);
	Audio::SoundManager::CreateInstance("BacgroundMusicLong.mp3", music);
	music->SetLoop(true);
	music->Set3D(false);
	music->Play();
}

/*
Each of the little demo scenarios used in the game uses the same 2 meshes, 
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!
*/
void Game::InitialiseAssets() {
	InitIntroWorld();
}

Game::~Game()	{
	delete resourceManager;
	delete networkManager;
	delete physics;
	delete renderer;
	delete world;
	delete music;
}

bool Game::UpdateGame(float dt) {

	if (gameStateMachine->Update(dt) == false)
		return false;

	
	//if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::K)) {
		//InitNetworkPlayers();
	//}

	UpdateKeys();
	
	if (!paused) {
		physics->Update(dt);

		world->UpdateWorld(dt);
	}

	UpdateKeys();
	renderer->Update(dt);

	if(networkManager)
		networkManager->Update(dt);

	Debug::FlushRenderables(dt);
	renderer->Render();
	Audio::SoundManager::Update();
	return true;
}

void Game::EnableNetworking(bool client) {

	if (!networkManager)
		networkManager = new NetworkManager(client);
	else {
		std::cout << "Attempted to enable networking whilst network manager is already active.\n";
	}
}
void Game::DisableNetworking() {
	if (networkManager) {
		delete networkManager;
		networkManager = nullptr;
	}
	else {
		std::cout << "Attempted to disable networking whilst no network manager is active.";
	}
}

bool NCL::CSC8508::Game::IsAllPlayersFinished()
{
	return !networkManager ? false : networkManager->IsAllPlayersFinished();
}

bool NCL::CSC8508::Game::IsMajorityPlayersFinished()
{
	return !networkManager ? false : networkManager->IsMajorityPlayersFinished();
}


GameObject* Game::Raycast(const Vector3& from, const Vector3& to) const {
	return physics->rayIntersect(from, to, Vector3());
}

void Game::UpdateKeys() {
//	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
//		InitIntroWorld(); //We can reset the simulation at any time with F1
//	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	//if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
	//	useGravity = !useGravity; //Toggle gravity!
		//physics->UseGravity(useGravity);
	//}
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

void Game::InitCamera() {
	AddCameraToWorld(Vector3(-60, 40, 60));
	CameraComponent::GetMain()->SetNearPlane(0.1f);
	CameraComponent::GetMain()->SetFarPlane(500.0f);
	CameraComponent::GetMain()->SetPitch(-15.0f);
	CameraComponent::GetMain()->SetYaw(315.0f);
}

void Game::InitIntroCamera() {
	AddCameraToWorld(Vector3(0, 0, 60));

	CameraComponent::GetMain()->SetNearPlane(0.1f);
	CameraComponent::GetMain()->SetFarPlane(500.0f);
	CameraComponent::GetMain()->SetPitch(0.0f);
	CameraComponent::GetMain()->SetYaw(0.0f);
}

void Game::Clear(bool force) {
	
	physics->clear();
	
	if (force)
		world->ForceClearAndErase();
	else
		world->ClearAndErase();

	Audio::SoundManager::Update();
}

void Game::InitFromJSON(std::string fileName) {
	JSONLevelFactory::ReadLevelFromJson(fileName, this);
}

void Game::InitNetworkPlayers()
{
//	GameObject* player = world->GetObjectWithTag("Player");
	//networkManager->SetLocalPlayer(player);

	GameObject* player = new GameObject("clientPlayer");
	world->AddGameObject(player);
	networkManager->SetLocalPlayer(player);
	player->AddComponent<LocalNetworkPlayerComponent>(networkManager->GetLocalPlayer());
	player->SetPersistence(true);

	std::queue<int>* lobby = networkManager->GetPlayerLobby();
	while (lobby->size() > 0) {
		int playerID = lobby->front();
		auto player = AddCapsuleToWorld(Vector3(5, 10, 10), 0.5f, 0.25f, 0);
		player->SetIsStatic(true);
		player->AddComponent<NetworkPlayerComponent>(playerID);
		player->AddComponent<PlayerRayFeetComponent>(this);
		player->SetIsActive(false);
		player->GetPhysicsObject()->body->makeKinematic();

		networkManager->AddPlayerToGame(playerID, player);
		std::cout << "Player " << std::to_string(lobby->front()) << " Added" << std::endl;

		lobby->pop();
	}

}

bool NCL::CSC8508::Game::IsExitLobbyTime()
{
	if (networkManager->IsClient())
		return networkManager->IsExitLobbyTime();
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::K)) {
		networkManager->ActivateExitLobby();
		return true;
	}
	return false;
}

void Game::InitWorld(std::string levelName, bool forceClear) {
	Clear(forceClear);

	InitCamera();

	InitFromJSON(levelName);

	//Tick the timer so that the load time isn't factored into any time related calculations
	Window::TickTimer();
}

void Game::InitIntroWorld() {
	Clear(true);
	InitIntroCamera();
}

GameObject* Game::AddCameraToWorld(const Vector3& position) {
	GameObject* camera = new GameObject("camera");
	camera->GetTransform().SetPosition(position);
	camera->AddComponent<CameraComponent>();
	world->AddGameObject(camera);
	return camera;
}

/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* Game::AddFloorToWorld(const Vector3& position) {
	GameObject* floor = new GameObject("floor");

	Vector3 floorSize	= Vector3(100, 2, 100);

	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), resourceManager->LoadMesh("cube.msh"),nullptr, resourceManager->LoadTexture("checkerboard.png"), nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	//testing adding bullet physics object
	floor->GetPhysicsObject()->body->addBoxShape(floorSize);
	floor->GetPhysicsObject()->body->createBody(0,
												0.4f,
												0.9f,
												physics);

	floor->GetPhysicsObject()->body->setUserPointer(floor);

	floor->SetIsStatic(true);

	world->AddGameObject(floor);

	return floor;
}

void Game::AddGameObject(GameObject* go)
{
	if (go != nullptr)
		world->AddGameObject(go);
}

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple' 
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* Game::AddSphereToWorld(const Vector3& position, float radius, float inverseMass) {
	GameObject* sphere = new GameObject("sphere");

	Vector3 sphereSize = Vector3(radius, radius, radius);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), resourceManager->LoadMesh("sphere.msh"),nullptr, resourceManager->LoadTexture("checkerboard.png"),nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	//testing adding bullet physics object
	sphere->GetPhysicsObject()->body->addSphereShape(radius);
	sphere->GetPhysicsObject()->body->createBody(	inverseMass,
													0.4f,
													0.4f,
													physics);
	sphere->GetPhysicsObject()->body->setUserPointer(sphere);

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* Game::AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass) {
	
	GameObject* capsule = new GameObject("capsule");

	capsule->GetTransform()
		.SetScale(Vector3(radius* 2, halfHeight + 0.05f, radius * 2))
		.SetPosition(position);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), resourceManager->LoadMesh("capsule.msh"),nullptr, resourceManager->LoadTexture("checkerboard.png"),nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->body->addCapsuleShape(radius,halfHeight);

	capsule->GetPhysicsObject()->body->createBody(	inverseMass,
													0.4f,
													0.4f,
													physics);
	capsule->GetPhysicsObject()->body->setUserPointer(capsule);
	
	world->AddGameObject(capsule);

	return capsule;

}

GameObject* Game::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, bool isStatic) {
	GameObject* cube = new GameObject("cube");

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), resourceManager->LoadMesh("cube.msh"),nullptr, resourceManager->LoadTexture("checkerboard.png"),nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	//testing adding bullet physics object
	cube->GetPhysicsObject()->body->addBoxShape(dimensions);
	cube->GetPhysicsObject()->body->createBody(	inverseMass,
												0.4f,
												0.4f,
												physics);
	cube->GetPhysicsObject()->body->setUserPointer(cube);

	world->AddGameObject(cube);

	return cube;
}

GameObject* Game::AddButtonToWorld(const Vector3& position, Vector3 dimensions,Vector4 colour, float inverseMass, bool isStatic) {
	GameObject* cube = new GameObject("cube");

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), resourceManager->LoadMesh("cube.msh"), nullptr, resourceManager->LoadTexture("level1/marble.png"),nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
	cube->GetRenderObject()->SetColour(colour);
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->SetIsStatic(isStatic);

	world->AddGameObject(cube);

	return cube;
}

GameObject* Game::AddOBBCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass , bool isStatic)
{
	GameObject* cube = new GameObject("cube");

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), resourceManager->LoadMesh("cube.msh"),nullptr, resourceManager->LoadTexture("checkerboard.png"),nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	//testing adding bullet physics object
	cube->GetPhysicsObject()->body->addBoxShape(dimensions);
	cube->GetPhysicsObject()->body->createBody(	inverseMass,
												0.4f,
												1.0f,
												physics);
	cube->GetPhysicsObject()->body->setUserPointer(cube);

	world->AddGameObject(cube);

	return cube;
}

void Game::InitDefaultFloor() {
	AddFloorToWorld(Vector3(0, -2, 0));
}



GameObject* Game::AddPlayerToWorld(const Vector3& position) {
	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	GameObject* character = new GameObject("player");

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * meshSize);

	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	if (rand() % 2) {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), resourceManager->LoadMesh("Male1.msh"),nullptr,  nullptr,nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
	}
	else {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), resourceManager->LoadMesh("courier.msh"),nullptr, nullptr,nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
	}
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	//lockedObject = character;

	return character;
}

GameObject* Game::AddEnemyToWorld(const Vector3& position) {
	float meshSize		= 3.0f;
	float inverseMass	= 0.5f;

	GameObject* character = new GameObject("enemy");

	AABBVolume* volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
	character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
		.SetScale(Vector3(meshSize, meshSize, meshSize))
		.SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), resourceManager->LoadMesh("security.msh"),nullptr, nullptr,nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
	character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	character->GetPhysicsObject()->SetInverseMass(inverseMass);
	character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	return character;
}

GameObject* Game::AddBonusToWorld(const Vector3& position) {
	GameObject* apple = new GameObject();

	SphereVolume* volume = new SphereVolume(0.25f);
	apple->SetBoundingVolume((CollisionVolume*)volume);
	apple->GetTransform()
		.SetScale(Vector3(0.25, 0.25, 0.25))
		.SetPosition(position);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), resourceManager->LoadMesh("coin.msh"),nullptr, nullptr,nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(1.0f);
	apple->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(apple);

	return apple;
}

