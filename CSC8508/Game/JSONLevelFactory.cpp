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
	if (transformJson.empty())
		return;

	transform.SetPosition(JSONShared::JsonToVector3(transformJson["position"]));
	transform.SetOrientation(JSONShared::JsonToQuaternion(transformJson["orientation"]));
	transform.SetScale(JSONShared::JsonToVector3(transformJson["scale"]));
}

void SetRenderObjectFromJson(GameObject* gameObject, json renderObjectJson, Game* game)
{
	if (renderObjectJson.empty())
		return;

	ResourceManager* resourceManager = game->GetResourceManager();

	MeshGeometry* mesh = resourceManager->LoadMesh(renderObjectJson["mesh"]);
	TextureBase* tex = resourceManager->LoadTexture(renderObjectJson["texture"]);
	ShaderBase* shader = resourceManager->LoadShader(renderObjectJson["vertex"],renderObjectJson["fragment"]);
	gameObject->GetTransform().SetScale(gameObject->GetTransform().GetScale() * renderObjectJson["renderScale"]);

	gameObject->SetRenderObject(new RenderObject(&gameObject->GetTransform(), mesh, tex, shader));
}

void SetPhysicsObjectFromJson(GameObject* gameObject, json physicsObjectJson)
{
	if (physicsObjectJson.empty())
		return;

	PhysicsObject* po = new PhysicsObject(&gameObject->GetTransform(), gameObject->GetBoundingVolume());
	po->SetInverseMass(physicsObjectJson["invMass"]);

	if (physicsObjectJson["inertia"] == "sphere")
		po->InitSphereInertia();
	else if (physicsObjectJson["inertia"] == "cube")
		po->InitCubeInertia();

	gameObject->SetPhysicsObject(po);
}

GameObject* CreateObjectFromJson(json objectJson, Game* game)
{
	GameObject* go = new GameObject(objectJson["name"]);
	Transform& transform = go->GetTransform();
	SetTransformFromJson(transform, objectJson["transform"]);

	if		(objectJson["collider"] == "AABB")		
		go->SetBoundingVolume(new AABBVolume(transform.GetScale()));
	else if (objectJson["collider"] == "OBB")		
		go->SetBoundingVolume(new OBBVolume(transform.GetScale()));
	else if (objectJson["collider"] == "Sphere")	
		go->SetBoundingVolume(new SphereVolume(transform.GetScale().x));

	SetRenderObjectFromJson(go, objectJson["render"], game);

	SetPhysicsObjectFromJson(go, objectJson["physics"]);

	go->SetIsStatic(objectJson["static"]);

	for (auto component : objectJson["components"])
	  JSONComponentFactory::AddComponentFromJson(component, go, game);

	return go;
}

void JSONLevelFactory::ReadLevelFromJson(std::string fileName, Game* game)
{
	std::ifstream input{ Assets::LEVELSDIR + fileName };

	json level {};

	input >> level;

	for (auto obj : level["objects"])
		game->AddGameObject(CreateObjectFromJson(obj,game));
}
