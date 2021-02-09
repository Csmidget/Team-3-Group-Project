#include "ObjectFactory.h"

#include "ForceObject.h"
#include "RespawningObject.h"
#include "PlayerObject.h"
#include "AIObject.h"
#include "ScoreBonusObject.h"
#include "BoidObject.h"
#include "Game.h"
#include "RaceAIBehaviourTree.h"
#include "KatamariAIBehaviourTree.h"

#include "../Engine/TraversableObject.h"
#include "../Engine/PositionConstraint.h"
#include "../Engine/OrientationConstraint.h"
#include "../Engine/AngularImpulseConstraint.h"

#include "../../Common/TextureLoader.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"

using namespace NCL;
using namespace CSC8508;
//hello
ObjectFactory::ObjectFactory() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh", &cubeMesh);
	loadFunc("sphere.msh", &sphereMesh);
	loadFunc("Male1.msh", &charMeshA);
	loadFunc("courier.msh", &charMeshB);
	loadFunc("security.msh", &enemyMesh);
	loadFunc("coin.msh", &coinMesh);
	loadFunc("apple.msh", &bonusMesh);
	loadFunc("capsule.msh", &capsuleMesh);

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");
}

ObjectFactory::~ObjectFactory() {
	delete cubeMesh;
	delete sphereMesh;
	delete charMeshA;
	delete charMeshB;
	delete enemyMesh;
	delete bonusMesh;

	delete basicTex;
	delete basicShader;
}


GameObject* ObjectFactory::CreateAABBCube(GameWorld* world, Vector3 position, Vector3 dimensions, float inverseMass, bool isStatic) const {
	GameObject* cube = new GameObject("cube");

	AABBVolume* volume = new AABBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();

	cube->SetIsStatic(isStatic);

	return world->AddGameObject(cube);
}

GameObject* ObjectFactory::CreateOBBCube(GameWorld* world, Vector3 position, Quaternion orientation, Vector3 dimensions, float inverseMass,bool respawning, bool isStatic) const {
	GameObject* cube = respawning ? new RespawningObject(position,"respawningOrientedCube",true) : new GameObject("orientedCube");

	OBBVolume* volume = new OBBVolume(dimensions);

	cube->SetBoundingVolume((CollisionVolume*)volume);

	cube->GetTransform()
		.SetPosition(position)
		.SetScale(dimensions * 2)
		.SetOrientation(orientation);

	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	cube->SetPhysicsObject(new PhysicsObject(&cube->GetTransform(), cube->GetBoundingVolume()));

	cube->GetPhysicsObject()->SetInverseMass(inverseMass);
	cube->GetPhysicsObject()->InitCubeInertia();
	cube->GetPhysicsObject()->SetElasticity(1.5f);
	cube->GetPhysicsObject()->SetFriction(0.7f);

	cube->SetIsStatic(isStatic);

	return world->AddGameObject(cube);
}
GameObject* ObjectFactory::CreateFloor(GameWorld* world, Vector3 position, Vector2 dimensions) const {
	GameObject* floor = new TraversableObject("floor");

	Vector3 floorSize = Vector3(dimensions.x, 0.5, dimensions.y);
	AABBVolume* volume = new AABBVolume(floorSize);
	floor->SetBoundingVolume((CollisionVolume*)volume);
	floor->GetTransform()
		.SetScale(floorSize * 2)
		.SetPosition(position);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();

	floor->SetIsStatic(true);

	return world->AddGameObject(floor);
}
GameObject* ObjectFactory::CreateOrientedFloor(GameWorld* world, Vector3 position, Quaternion orientation, Vector2 dimensions, std::string name) const {
	GameObject* floor = new TraversableObject(name);

	Vector3 floorSize = Vector3(dimensions.x, 0.5f, dimensions.y);
	OBBVolume* volume = new OBBVolume(Vector3(floorSize));

	floor->SetBoundingVolume((CollisionVolume*)volume);

	floor->GetTransform()
		.SetPosition(position)
		.SetScale(floorSize * 2)
		.SetOrientation(orientation);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();
	floor->GetPhysicsObject()->SetFriction(0.2f);

	floor->SetIsStatic(true);

	return world->AddGameObject(floor);
}

GameObject* ObjectFactory::CreateSlipperyFloor(GameWorld* world, const Vector3& position, const Quaternion& orientation,const Vector2& dimensions) const {
	GameObject* floor = CreateOrientedFloor(world, position, orientation, dimensions,"slipperyFloor");
	PhysicsObject* phys = floor->GetPhysicsObject();

	phys->SetFriction(0.0f);
	
	floor->GetRenderObject()->SetColour(Vector4(5, 5, 5, 1));

	return floor;
}

GameObject* ObjectFactory::CreateSwampFloor(GameWorld* world, const Vector3& position, const Quaternion& orientation, const Vector2& dimensions) const {
	GameObject* floor = new TraversableObject("swampFloor",'o');

	Vector3 floorSize = Vector3(dimensions.x, 0.7f, dimensions.y);

	//Swamps collider is slightly thinner than its rendered size, to give the appearance of sinking in mud.
	OBBVolume* volume = new OBBVolume(Vector3(dimensions.x, 0.5f, dimensions.y));

	floor->SetBoundingVolume((CollisionVolume*)volume);

	floor->GetTransform()
		.SetPosition(position)
		.SetScale(floorSize * 2)
		.SetOrientation(orientation);

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->GetRenderObject()->SetColour(Vector4(0.545f, 0.27f, 0.075f, 1));

	floor->SetPhysicsObject(new PhysicsObject(&floor->GetTransform(), floor->GetBoundingVolume()));

	floor->GetPhysicsObject()->SetInverseMass(0);
	floor->GetPhysicsObject()->InitCubeInertia();
	floor->GetPhysicsObject()->SetFriction(1.5f);

	floor->SetIsStatic(true);

	return world->AddGameObject(floor);
}

GameObject* ObjectFactory::CreateCapsule(GameWorld* world, Vector3 position, Quaternion orientation, float halfHeight, float radius, float inverseMass, bool respawning, bool isStatic) const {
	GameObject* capsule = respawning ? new RespawningObject(position,"respawningCapsule",true) : new GameObject("capsule");

	CapsuleVolume* volume = new CapsuleVolume(halfHeight, radius);
	capsule->SetBoundingVolume((CollisionVolume*)volume);

	capsule->GetTransform()
		.SetScale(Vector3(radius * 2, halfHeight, radius * 2))
		.SetPosition(position);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTex, basicShader));
	capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	capsule->GetPhysicsObject()->InitCubeInertia();
	capsule->GetPhysicsObject()->SetElasticity(1.0f);
	capsule->GetPhysicsObject()->SetFriction(0.5f);

	capsule->SetIsStatic(isStatic);

	return world->AddGameObject(capsule);
}

GameObject* ObjectFactory::CreateSphere(GameWorld* world, Vector3 position, float radius, float inverseMass, bool respawning, bool isStatic) const {
	GameObject* sphere = respawning ? new RespawningObject(position,"respawningSphere",true) : new GameObject("sphere");

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	sphere->SetBoundingVolume((CollisionVolume*)volume);

	sphere->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));
	sphere->SetPhysicsObject(new PhysicsObject(&sphere->GetTransform(), sphere->GetBoundingVolume()));

	sphere->GetPhysicsObject()->SetInverseMass(inverseMass);
	sphere->GetPhysicsObject()->InitHollowSphereInertia();
	sphere->GetPhysicsObject()->SetElasticity(1.0f);
	sphere->GetPhysicsObject()->SetFriction(0.4f);


	sphere->SetIsStatic(isStatic);

	return world->AddGameObject(sphere);
}

GameObject* ObjectFactory::CreateAnchor(GameWorld* world, const Vector3& position) const {
	GameObject* anchor = new GameObject("anchor");

	anchor->GetTransform().SetPosition(position);

	anchor->SetPhysicsObject(new PhysicsObject(&anchor->GetTransform(), nullptr));
	anchor->GetPhysicsObject()->SetInverseMass(0.0f);
	anchor->SetIsStatic(true);

	return world->AddGameObject(anchor);
}

//This creates an invisible and non physical square that blocks path generation rays, essentially blocking AI from entering certain areas.
GameObject* ObjectFactory::CreatePathBlocker(GameWorld* world, const Vector2& position, const Vector2& halfDimensions) const {
	GameObject* pathBlocker = new GameObject("pathBlocker");

	Vector3 halfDims = Vector3(halfDimensions.x, 0.01f, halfDimensions.y);
	pathBlocker->SetBoundingVolume(new AABBVolume(halfDims));

	Vector3 pos(position.x, 900, position.y);
	pathBlocker->GetTransform()
		.SetPosition(pos)
		.SetScale(halfDims * 2);

	pathBlocker->SetIsStatic(true);

	return world->AddGameObject(pathBlocker);
}

GameObject* ObjectFactory::CreateTreadmill(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength, const Vector2& dimensions) const {
	GameObject* treadmill = new ForceObject(world, position, Vector3(dimensions.x, 0.5f, dimensions.y), orientation, Vector3(0, 0, -1), strength);

	treadmill->SetRenderObject( new RenderObject(&treadmill->GetTransform(), cubeMesh, basicTex, basicShader));
	treadmill->GetRenderObject()->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1.0f));

	return world->AddGameObject(treadmill);
}

GameObject* ObjectFactory::CreateBouncePad(GameWorld* world, const Vector3& position, const Quaternion& orientation, float strength, const Vector2& dimensions) const {

	GameObject* bouncePad = new ForceObject(world, position, Vector3(dimensions.x, 0.5f, dimensions.y), orientation, Vector3(0, 1, 0), strength,false);

	bouncePad->SetRenderObject(new RenderObject(&bouncePad->GetTransform(), cubeMesh, basicTex, basicShader));
	bouncePad->GetRenderObject()->SetColour(Vector4(0.2f, 0.2f, 0.2f, 1.0f));

	return world->AddGameObject(bouncePad);
}

GameObject* ObjectFactory::CreateSpinningBlock(GameWorld* world, const Vector3& position, const Vector3& upVector, float force, float length) const {

	GameObject* spinningBlock = CreateOBBCube(world, position, Quaternion(), Vector3(length, 2, 2), 0.01f);

	GameObject* positionAnchor = CreateAnchor(world, position);
	world->AddConstraint(new PositionConstraint(positionAnchor, spinningBlock, 0.0f));

	GameObject* orientationAnchor = CreateAnchor(world, position + upVector);
	world->AddConstraint(new OrientationConstraint(spinningBlock, orientationAnchor, Vector3(0, 1, 0)));

	world->AddConstraint(new AngularImpulseConstraint(spinningBlock, Vector3(0,force,0)));

	return spinningBlock;
}

PlayerObject* ObjectFactory::CreatePlayer(Game* game, const Vector3& position) const {
	float meshSize = 0.8f;
	float inverseMass = 5.0f;

	PlayerObject* player = new PlayerObject(game, position);

	CapsuleVolume* volume = new CapsuleVolume(meshSize,0.3f);
	
	player->SetBoundingVolume((CollisionVolume*)volume);
	
	float modelScale = 1.15f;
	player->GetTransform()
		.SetScale(Vector3(meshSize * modelScale, meshSize * modelScale, meshSize * modelScale))
		.SetPosition(position);
	
	if (rand() % 2) {
		player->SetRenderObject(new RenderObject(&player->GetTransform(), charMeshA, nullptr, basicShader));
	}
	else {
		player->SetRenderObject(new RenderObject(&player->GetTransform(), charMeshB, nullptr, basicShader));
	}
	player->SetPhysicsObject(new PhysicsObject(&player->GetTransform(), player->GetBoundingVolume()));
	
	player->GetPhysicsObject()->SetInverseMass(inverseMass);
	player->GetPhysicsObject()->InitCubeInertia();
	player->GetPhysicsObject()->SetElasticity(0.8f);
	player->GetPhysicsObject()->SetFriction(0.1f);
	
	game->GetWorld()->AddGameObject(player);
		
	return player;
}

AIObject* ObjectFactory::CreateRaceAI(Game* game, const Vector3& position, std::string name, float coinDetectionRange, float maxCoinDistance, float angerThreshold, float strength) const {
	float meshSize = 0.8f;
	float inverseMass = 5.0f;

	AIObject* aiPlayer = new AIObject(game, position, name, coinDetectionRange, maxCoinDistance, angerThreshold, strength);

	aiPlayer->SetBehaviourTree(new RaceAIBehaviourTree(game,aiPlayer));

	CapsuleVolume* volume = new CapsuleVolume(meshSize, 0.3f);

	aiPlayer->SetBoundingVolume((CollisionVolume*)volume);

	float modelScale = 1.15f;
	Transform& transform = aiPlayer->GetTransform();
	transform
		.SetScale(Vector3(meshSize * modelScale, meshSize * modelScale, meshSize * modelScale))
		.SetPosition(position);

	if (rand() % 2) {
		aiPlayer->SetRenderObject(new RenderObject(&transform, charMeshA, nullptr, basicShader));
	}
	else {
		aiPlayer->SetRenderObject(new RenderObject(&transform, charMeshB, nullptr, basicShader));
	}
	aiPlayer->SetPhysicsObject(new PhysicsObject(&transform, aiPlayer->GetBoundingVolume()));

	aiPlayer->GetPhysicsObject()->SetInverseMass(inverseMass);
	aiPlayer->GetPhysicsObject()->InitCubeInertia();
	aiPlayer->GetPhysicsObject()->SetElasticity(0.8f);
	aiPlayer->GetPhysicsObject()->SetFriction(0.1f);

	aiPlayer->UpdateAngerColour();

	game->GetWorld()->AddGameObject(aiPlayer);

	return aiPlayer;
}

AIObject* ObjectFactory::CreateKatamariAI(Game* game, const Vector3& position, std::string name) const {
	float meshSize = 0.8f;
	float inverseMass = 5.0f;

	AIObject* aiPlayer = new AIObject(game, position, name,0.0f,0.0f,0.0f,3.0f);

	aiPlayer->SetSpeed(5.0f);
	aiPlayer->SetBehaviourTree(new KatamariAIBehaviourTree(game, aiPlayer));

	CapsuleVolume* volume = new CapsuleVolume(meshSize, 0.3f);

	aiPlayer->SetBoundingVolume((CollisionVolume*)volume);

	float modelScale = 1.15f;
	Transform& transform = aiPlayer->GetTransform();
	transform
		.SetScale(Vector3(meshSize * modelScale, meshSize * modelScale, meshSize * modelScale))
		.SetPosition(position);


	if (rand() % 2) {
		aiPlayer->SetRenderObject(new RenderObject(&transform, charMeshA, nullptr, basicShader));
	}
	else {
		aiPlayer->SetRenderObject(new RenderObject(&transform, charMeshB, nullptr, basicShader));
	}
	aiPlayer->SetPhysicsObject(new PhysicsObject(&transform, aiPlayer->GetBoundingVolume()));

	aiPlayer->GetPhysicsObject()->SetInverseMass(inverseMass);
	aiPlayer->GetPhysicsObject()->InitCubeInertia();
	aiPlayer->GetPhysicsObject()->SetElasticity(0.8f);
	aiPlayer->GetPhysicsObject()->SetFriction(0.1f);

	aiPlayer->UpdateAngerColour();

	game->GetWorld()->AddGameObject(aiPlayer);

	return aiPlayer;
}

GameObject* ObjectFactory::CreateScoreBonus(GameWorld* world, Vector3 position,float respawnDelay) const {

	ScoreBonusObject* scoreObject = new ScoreBonusObject(position,25, respawnDelay);

	SphereVolume* volume = new SphereVolume(1.0f);
	scoreObject->SetBoundingVolume((CollisionVolume*)volume);
	scoreObject->GetTransform()
		.SetScale(Vector3(0.1f,0.1f,0.1f))
		.SetPosition(position);

	scoreObject->SetRenderObject(new RenderObject(&scoreObject->GetTransform(), coinMesh, nullptr, basicShader));
	scoreObject->GetRenderObject()->SetColour(Vector4(1, 1, 0, 1));

	world->AddGameObject(scoreObject);

	return scoreObject;
}

BoidObject* ObjectFactory::CreateBoid(Game* game, BoidSwarm** swarm, const Vector3& position) const {

	BoidObject* boid = new BoidObject(game, swarm, position);

	float radius = 0.25f;

	Vector3 sphereSize = Vector3(radius, radius, radius);
	SphereVolume* volume = new SphereVolume(radius);
	boid->SetBoundingVolume((CollisionVolume*)volume);

	boid->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position);

	boid->SetRenderObject(new RenderObject(&boid->GetTransform(), sphereMesh, basicTex, basicShader));
	boid->SetPhysicsObject(new PhysicsObject(&boid->GetTransform(), boid->GetBoundingVolume()));

	boid->GetPhysicsObject()->SetInverseMass(20.0f);
	boid->GetPhysicsObject()->InitHollowSphereInertia();
	boid->GetPhysicsObject()->SetElasticity(1.0f);
	boid->GetPhysicsObject()->SetFriction(0.4f);

	game->GetWorld()->AddGameObject(boid);

	return boid;
}
