#include "JSONLevelFactory.h"
#include "JSONComponentFactory.h"
#include "JSONShared.h"
#include "Game.h"

#include "../Engine/GameObject.h"
#include "../Engine/CollisionDetection.h"
#include "../../Plugins/json/json.hpp"
#include "../../Common/Assets.h"

#include <fstream>

using namespace NCL;
using namespace CSC8508;

using json = nlohmann::json;

void SetTransformFromJson(Transform& transform, json transformJson)
{
	if (!transformJson.is_object())
		return;

	transform.SetPosition(JSONShared::JsonToVector3(transformJson["position"]));
	transform.SetOrientation(JSONShared::JsonToQuaternion(transformJson["orientation"]));
	transform.SetScale(JSONShared::JsonToVector3(transformJson["scale"]));
}

void SetRenderObjectFromJson(GameObject* gameObject, json renderObjectJson, Game* game)
{
	if (!renderObjectJson.is_object())
		return;

	ResourceManager* resourceManager = game->GetResourceManager();

	MeshGeometry* mesh = resourceManager->LoadMesh(renderObjectJson["mesh"]);

	MeshMaterial* meshMat = nullptr;
	if (renderObjectJson["material"].is_string())
		meshMat = resourceManager->LoadMaterial(renderObjectJson["material"]);

	TextureBase* tex = nullptr;

	if (renderObjectJson["texture"].is_string())
		tex = resourceManager->LoadTexture(renderObjectJson["texture"]);
	else
		tex = resourceManager->LoadTexture("checkerboard.png");

	ShaderBase* shader = resourceManager->LoadShader("GameTechVert.glsl", "GameTechFrag.glsl");//renderObjectJson["vertex"],renderObjectJson["fragment"]);
	gameObject->GetTransform().SetScale(gameObject->GetTransform().GetScale() * renderObjectJson["renderScale"]);

	gameObject->SetRenderObject(new RenderObject(&gameObject->GetTransform(), mesh, meshMat, tex, shader));
}

void SetPhysicsObjectFromJson(GameObject* gameObject, json physicsObjectJson)
{
	if (!physicsObjectJson.is_object())
		return;

	PhysicsObject* po = new PhysicsObject(&gameObject->GetTransform(), gameObject->GetBoundingVolume());
	po->SetInverseMass(physicsObjectJson["invMass"]);

	if (physicsObjectJson["inertia"] == "sphere")
		po->InitSphereInertia();
	else if (physicsObjectJson["inertia"] == "cube")
		po->InitCubeInertia();
	else
		po->InitCubeInertia();

	gameObject->SetPhysicsObject(po);
}

void SetColliderFromJson(GameObject* gameObject, json colliderJson)
{
	if (!colliderJson.is_object())
		return;

	CollisionVolume* volume = nullptr;
	Transform& transform = gameObject->GetTransform();

	if (colliderJson["type"] == "box")
		volume = new AABBVolume(transform.GetScale());
	else if (colliderJson["type"] == "obbbox")
		volume = new OBBVolume(transform.GetScale());
	else if (colliderJson["type"] == "sphere")
		volume = new SphereVolume(transform.GetScale().x);
	else if (colliderJson["type"] == "capsule")
		volume = new CapsuleVolume(colliderJson["halfHeight"],colliderJson["radius"]);

	gameObject->SetBoundingVolume(volume);
}

GameObject* CreateObjectFromJson(json objectJson, Game* game)
{
	if (!objectJson.is_object())
		return nullptr;
	
	GameObject* go = (objectJson["name"].is_string() ? new GameObject(objectJson["name"]) : new GameObject("unnamed"));
	Transform& transform = go->GetTransform();

	SetTransformFromJson(transform, objectJson["transform"]);

	SetColliderFromJson(go, objectJson["collider"]);

	SetRenderObjectFromJson(go, objectJson["render"], game);

	SetPhysicsObjectFromJson(go, objectJson["physics"]);

	go->SetIsStatic(objectJson["static"].is_boolean() ? objectJson["static"] : false);

	for (auto component : objectJson["components"])
	  JSONComponentFactory::AddComponentFromJson(component, go, game);

	return go;
}

void JSONLevelFactory::ReadLevelFromJson(std::string fileName, Game* game)
{
	std::ifstream input{ Assets::LEVELSDIR + fileName };

	json level {};

	input >> level;

	if (!level.is_object())
		throw std::exception("Unable to read level json");

	for (auto obj : level["objects"])
		game->AddGameObject(CreateObjectFromJson(obj,game));
}
