#include "Game.h"
#include "JSONLevelFactory.h"
#include "GameTechRenderer.h"
#include "../Engine/PhysicsSystem.h"
#include "../Engine/GameWorld.h"
#include "../../Plugins/OpenGLRendering/OGLResourceManager.h"
#include "../../Common/TextureLoader.h"
#include "../Engine/PositionConstraint.h"
#include "../Engine/OrientationConstraint.h"
#include "RespawningObject.h"
#include"../Audio/SoundManager.h"
#include"../Audio/SoundInstance.h"
#include "../../Common/ShaderBase.h"

#include "PlayerComponent.h"
#include "RespawnComponent.h"

//JENKINS TEST 3

using namespace NCL;
using namespace CSC8508;
using namespace Maths;

Game::Game() {
	resourceManager = new OGLResourceManager();
	world = new GameWorld();
	renderer = new GameTechRenderer(*world, *resourceManager);
	//physics		= new PhysicsSystem(*world);
	physics		= new physics::BulletWorld();

	networkManager = new NetworkManager();

	forceMagnitude = 10.0f;
	useGravity = false;
	inSelectionMode = false;

	Debug::SetRenderer(renderer);
	Audio::SoundManager::Init();
	InitialiseAssets();
	Audio::SoundInstance* test = new Audio::SoundInstance();
	test->SetVolume(0.1f);
	Audio::SoundManager::CreateInstance("River.mp3", test);
	test->Play();
}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes, 
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void Game::InitialiseAssets() {

	//Todo: These should be removed whenever we fully shift to json levels.

	InitCamera();
	InitWorld();
}

Game::~Game()	{
	delete resourceManager;
	delete networkManager;
	delete physics;
	delete renderer;
	delete world;
}

void Game::UpdateGame(float dt) {
	if (!inSelectionMode) {
		world->GetMainCamera()->UpdateCamera(dt);
	}

	UpdateKeys();

	if (useGravity) {
		Debug::Print("(G)ravity on", Vector2(5, 95));
	}
	else {
		Debug::Print("(G)ravity off", Vector2(5, 95));
	}

	SelectObject();
	MoveSelectedObject();
	physics->Update(dt);

	if (lockedObject != nullptr) {
		Vector3 objPos = lockedObject->GetTransform().GetPosition();
		Vector3 camPos = objPos + lockedOffset;

		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0,1,0));

		Matrix4 modelMat = temp.Inverse();

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!

		world->GetMainCamera()->SetPosition(camPos);
		world->GetMainCamera()->SetPitch(angles.x);
		world->GetMainCamera()->SetYaw(angles.y);

		//Debug::DrawAxisLines(lockedObject->GetTransform().GetMatrix(), 2.0f);
	}

	world->UpdateWorld(dt);
	renderer->Update(dt);
	networkManager->Update(dt);

	Debug::FlushRenderables(dt);
	renderer->Render();
	Audio::SoundManager::Update();
}

void Game::UpdateKeys() {
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F1)) {
		InitWorld(); //We can reset the simulation at any time with F1
		selectionObject = nullptr;
		lockedObject	= nullptr;
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::F2)) {
		InitCamera(); //F2 will reset the camera to a specific default place
	}

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::G)) {
		useGravity = !useGravity; //Toggle gravity!
		//physics->UseGravity(useGravity);
	}
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

	if (lockedObject) {
		LockedObjectMovement();
	}
	else {
		DebugObjectMovement();
	}
}

void Game::LockedObjectMovement() {
	Matrix4 view		= world->GetMainCamera()->BuildViewMatrix();
	Matrix4 camWorld	= view.Inverse();

	Vector3 rightAxis = Vector3(camWorld.GetColumn(0)); //view is inverse of model!

	//forward is more tricky -  camera forward is 'into' the screen...
	//so we can take a guess, and use the cross of straight up, and
	//the right axis, to hopefully get a vector that's good enough!

	Vector3 fwdAxis = Vector3::Cross(Vector3(0, 1, 0), rightAxis);
	fwdAxis.y = 0.0f;
	fwdAxis.Normalise();

	Vector3 charForward  = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);
	Vector3 charForward2 = lockedObject->GetTransform().GetOrientation() * Vector3(0, 0, 1);

	float force = 100.0f;
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::A)) {
		lockedObject->GetPhysicsObject()->AddForce(-rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::D)) {
		lockedObject->GetPhysicsObject()->AddForce(rightAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::W)) {
		lockedObject->GetPhysicsObject()->AddForce(fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::S)) {
		lockedObject->GetPhysicsObject()->AddForce(-fwdAxis * force);
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::SPACE)) {
		lockedObject->GetPhysicsObject()->AddForce(Vector3(0,600.0f,0));
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::R)) {
		selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -100, 0));
	}

	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::E)) {
		selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 100, 0));
	}

}

void Game::DebugObjectMovement() {
//If we've selected an object, we can manipulate it with some key presses
	if (inSelectionMode && selectionObject) {
		//Twist the selected object!
		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::LEFT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(-10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM7)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, 10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM8)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(0, -10, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::RIGHT)) {
			selectionObject->GetPhysicsObject()->AddTorque(Vector3(10, 0, 0));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::UP)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, -10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::DOWN)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 10));
		}

		if (Window::GetKeyboard()->KeyDown(KeyboardKeys::NUM5)) {
			selectionObject->GetPhysicsObject()->AddForce(Vector3(0, -10, 0));
		}
	}

}

void Game::InitCamera() {
	world->GetMainCamera()->SetNearPlane(0.1f);
	world->GetMainCamera()->SetFarPlane(500.0f);
	world->GetMainCamera()->SetPitch(-15.0f);
	world->GetMainCamera()->SetYaw(315.0f);
	world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));
	lockedObject = nullptr;
}

void Game::Clear() {
	world->ClearAndErase();
	physics->clear();

	useGravity = true;
	//physics->UseGravity(true);
}

void Game::InitFromJSON(std::string fileName) {
	Clear();
	JSONLevelFactory::ReadLevelFromJson(fileName, this);
}

void Game::InitWorld() {
	Clear();

	InitFromJSON("CharlesTest.json");

	auto player = AddCapsuleToWorld(Vector3(10, 10, 10), 1.0f, 0.5f, 1.0f, false);
	player->AddComponent<PlayerComponent>(this);
	player->AddComponent<RespawnComponent>();
	world->Start();

	world->AddKillPlane(new Plane(Vector3(0, 1, 0), Vector3(0, -5, 0)));

	//AddFloorToWorld(Vector3(0, 0, 0));
	//AddCubeToWorld(Vector3(0, 30, 0), Vector3(1, 1, 1), 10);
	//AddSphereToWorld(Vector3(10, 10, 0), 1.0f, 10);
	//AddSphereToWorld(Vector3(9.8f, 20, 0), 1.0f, 10);
	//AddCapsuleToWorld(Vector3(20, 10, 0), 1.0, 0.5, 10.0f);
}

void Game::DoorConstraintTest() {
	GameObject* hinge = AddOBBCubeToWorld(Vector3(-30, 19, -30), Vector3(1, 1, 1), 0.0f, true);
	GameObject* hinge2 = AddOBBCubeToWorld(Vector3(-30, 30, -40), Vector3(1, 1, 1), 0.0f, true);
	GameObject* door = AddOBBCubeToWorld(Vector3(-30, 20, -40), Vector3(2, 2, 2), 10.0f, false);
	world->AddConstraint(new OrientationConstraint(door, hinge, Vector3(0, 1, 0)));
	world->AddConstraint(new PositionConstraint(hinge, door, 10.0f));
}

void Game::BridgeConstraintTest() {
	Vector3 cubeSize = Vector3(8, 8, 8);

	float invCubeMass = 5;
	int numLinks = 10;
	float maxDistance = 30;
	float cubeDistance = 20;

	Vector3 startPos = Vector3(50, 20, 50);

	GameObject* start = AddCubeToWorld(startPos + Vector3(0, 0, 0), cubeSize, 0,true);
	GameObject* end = AddCubeToWorld(startPos + Vector3((numLinks + 2) * cubeDistance, 0, 0), cubeSize, 0,true);

	GameObject* previous = start;

	for (int i = 0; i < numLinks; ++i) {
		GameObject* block = AddCubeToWorld(startPos + Vector3((i + 1) * cubeDistance, 0, 0), cubeSize, invCubeMass);
		PositionConstraint* constraint = new PositionConstraint(previous, block, maxDistance);

		world->AddConstraint(constraint);
		previous = block;
	}

	PositionConstraint* constraint = new PositionConstraint(previous, end, maxDistance);

	world->AddConstraint(constraint);
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

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), resourceManager->LoadMesh("cube.msh"),nullptr, resourceManager->LoadTexture("checkerboard.png"), resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
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
GameObject* Game::AddSphereToWorld(const Vector3& position, float radius, float inverseMass, bool respawning) {
	GameObject* sphere = respawning ? new RespawningObject(position,true,"respawning sphere") : new GameObject("sphere");

	Vector3 sphereSize = Vector3(radius, radius, radius);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), resourceManager->LoadMesh("sphere.msh"),nullptr, resourceManager->LoadTexture("checkerboard.png"), resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
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

GameObject* Game::AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass, bool respawning) {
	
	GameObject* capsule = respawning ? new RespawningObject(position,true,"respawning_capsule") : new GameObject("capsule");

	capsule->GetTransform()
		.SetScale(Vector3(radius* 2, halfHeight, radius * 2))
		.SetPosition(position);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), resourceManager->LoadMesh("capsule.msh"),nullptr, resourceManager->LoadTexture("checkerboard.png"), resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
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

GameObject* Game::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass, bool isStatic, bool respawning) {
	GameObject* cube = respawning ? new RespawningObject(position,true,"respawning cube") : new GameObject("cube");

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), resourceManager->LoadMesh("cube.msh"),nullptr, resourceManager->LoadTexture("checkerboard.png"), resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
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

GameObject* Game::AddOBBCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass , bool isStatic, bool respawning)
{
	GameObject* cube = respawning ? new RespawningObject(position, true, "respawning cube") : new GameObject("cube");

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), resourceManager->LoadMesh("cube.msh"),nullptr, resourceManager->LoadTexture("checkerboard.png"), resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
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

void Game::InitSphereGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, float radius) {
	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddSphereToWorld(position, radius, 1.0f);
		}
	}
	AddFloorToWorld(Vector3(0, -2, 0));
}

void Game::InitMixedGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing) {
	float sphereRadius = 1.0f;
	Vector3 cubeDims = Vector3(1, 1, 1);

	for (int x = 0; x < numCols; ++x) {
		for (int z = 0; z < numRows; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			int typeInt = rand() % 3;
			if (typeInt == 0) {
				AddCubeToWorld(position, cubeDims);
			}
			else if (typeInt == 1) {
				AddSphereToWorld(position, sphereRadius);
			}
			else {
				AddCapsuleToWorld(position, 1, 0.5);
			}
		}
	}
}

void Game::InitCubeGridWorld(int numRows, int numCols, float rowSpacing, float colSpacing, const Vector3& cubeDims) {
	for (int x = 1; x < numCols+1; ++x) {
		for (int z = 1; z < numRows+1; ++z) {
			Vector3 position = Vector3(x * colSpacing, 10.0f, z * rowSpacing);
			AddCubeToWorld(position, cubeDims, 1.0f);
		}
	}
}

void Game::InitDefaultFloor() {
	AddFloorToWorld(Vector3(0, -2, 0));
}

void Game::InitGameExamples() {
	AddPlayerToWorld(Vector3(0, 5, 0));
	AddEnemyToWorld(Vector3(5, 5, 0));
	AddBonusToWorld(Vector3(10, 5, 0));
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
		character->SetRenderObject(new RenderObject(&character->GetTransform(), resourceManager->LoadMesh("Male1.msh"),nullptr,  nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
	}
	else {
		character->SetRenderObject(new RenderObject(&character->GetTransform(), resourceManager->LoadMesh("courier.msh"),nullptr, nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
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

	character->SetRenderObject(new RenderObject(&character->GetTransform(), resourceManager->LoadMesh("security.msh"),nullptr, nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
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

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), resourceManager->LoadMesh("coin.msh"),nullptr, nullptr, resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl")));
	apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	apple->GetPhysicsObject()->SetInverseMass(1.0f);
	apple->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(apple);

	return apple;
}

/*

Every frame, this code will let you perform a raycast, to see if there's an object
underneath the cursor, and if so 'select it' into a pointer, so that it can be 
manipulated later. Pressing Q will let you toggle between this behaviour and instead
letting you move the camera around. 

*/
bool Game::SelectObject() {

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::P)) {
		world->ClearAndErase();
		//physics->Clear();
	}
	

	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::Q)) {
		inSelectionMode = !inSelectionMode;
		if (inSelectionMode) {
			Window::GetWindow()->ShowOSPointer(true);
			Window::GetWindow()->LockMouseToWindow(false);
		}
		else {
			Window::GetWindow()->ShowOSPointer(false);
			Window::GetWindow()->LockMouseToWindow(true);
		}
	}
	if (inSelectionMode) {
		renderer->DrawString("Press Q to change to camera mode!", Vector2(5, 85));

		if (Window::GetMouse()->ButtonDown(NCL::MouseButtons::LEFT)) {
			if (selectionObject) {	//set colour to deselected;
				selectionObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));

				if(forwardObject)
					forwardObject->GetRenderObject()->SetColour(Vector4(1, 1, 1, 1));

				selectionObject = nullptr;
				forwardObject = nullptr;
				lockedObject	= nullptr;
			}

			Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
			//RayCollision closestCollision;
			Vector3 testVector1;
			GameObject* test = physics->rayIntersect(ray.GetPosition(), ray.GetDirection() * 5000.0f, testVector1);
			if(test)
				std::cout << test->GetName() << std::endl;

			if (test) {
				//need to think out where the debug line draws now
				//Debug::DrawLine(ray.GetPosition(),testVector1, Vector4(0, 1, 0, 1), 10.0f);
				selectionObject = test;
				selectionObject->GetRenderObject()->SetColour(Vector4(0, 1, 0, 1));
				
				ray = Ray(selectionObject->GetTransform().GetPosition(), selectionObject->GetTransform().GetOrientation() * Vector3(0, 0, -1));
				Vector3 testVector2;
				GameObject* test2 = physics->rayIntersect(ray.GetPosition(), ray.GetDirection() * 5000.0f, testVector2);
				if (test2) {
					//Debug::DrawLine(ray.GetPosition(), test2->GetTransform().GetPosition(), Vector4(1, 1, 0, 1), 10.0f);
					forwardObject = test2;
					forwardObject->GetRenderObject()->SetColour(Vector4(1, 1, 0, 1));
				}
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		renderer->DrawString("Press Q to change to select mode!", Vector2(5, 85));
	}



	if (lockedObject) {
		renderer->DrawString("Press L to unlock object!", Vector2(5, 80));
	}

	else if(selectionObject){
		renderer->DrawString("Press L to lock selected object object!", Vector2(5, 80));
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::L)) {
		if (selectionObject) {
			if (lockedObject == selectionObject) {
				lockedObject = nullptr;
			}
			else {
				lockedObject = selectionObject;
			}
		}

	}

	return false;
}

/*
If an object has been clicked, it can be pushed with the right mouse button, by an amount
determined by the scroll wheel. In the first tutorial this won't do anything, as we haven't
added linear motion into our physics system. After the second tutorial, objects will move in a straight
line - after the third, they'll be able to twist under torque aswell.
*/
void Game::MoveSelectedObject() {
//	renderer->DrawString("Click Force: " + std::to_string(forceMagnitude), Vector2(10, 20));
	forceMagnitude += Window::GetMouse()->GetWheelMovement() * 1.0f;

	if (!selectionObject) {
		return;
	}

	if (Window::GetMouse()->ButtonPressed(NCL::MouseButtons::RIGHT)) {
		Ray ray = CollisionDetection::BuildRayFromMouse(*world->GetMainCamera());
		RayCollision closestCollision;

		if (world->Raycast(ray, closestCollision, true)) {
			if (closestCollision.node == selectionObject) {
				selectionObject->GetPhysicsObject()->AddForceAtPosition(ray.GetDirection() * forceMagnitude, closestCollision.collidedAt);
			}
		}
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::UP)) {
		selectionObject->GetPhysicsObject()->AddForce(Vector3(0,0,-1) * forceMagnitude);
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::DOWN)) {
		selectionObject->GetPhysicsObject()->AddForce(Vector3(0, 0, 1) * forceMagnitude);
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::LEFT)) {
		selectionObject->GetPhysicsObject()->AddForce(Vector3(-1, 0, 0) * forceMagnitude); 
	}

	if (Window::GetKeyboard()->KeyPressed(NCL::KeyboardKeys::RIGHT)) {
		selectionObject->GetPhysicsObject()->AddForce(Vector3(1, 0, 0) * forceMagnitude);
	}

}

