#include "JsonReader.h"
#include "Game.h"
#include "../Engine/GameObject.h"
#include "../Engine/CollisionDetection.h"
#include "../../Plugins/json/json.hpp"

#include <fstream>

using namespace NCL;
using namespace CSC8508;

using json = nlohmann::json;

Vector3 JsonToVector3(json vectorJson)
{
	return { vectorJson["x"],vectorJson["y"],vectorJson["z"] };
}

Quaternion JsonToQuaternion(json quatJson)
{
	return { quatJson["x"],quatJson["y"],quatJson["z"],quatJson["w"] };
}

void SetTransformFromJson(Transform& transform, json transformJson)
{
	transform.SetPosition(JsonToVector3(transformJson["position"]));
	transform.SetOrientation(JsonToQuaternion(transformJson["orientation"]));
	transform.SetScale(JsonToVector3(transformJson["scale"]));
}

void SetRenderObjectFromJson(GameObject* gameObject, json renderObjectJson, Game* game)
{
	ResourceManager* resourceManager = game->GetResourceManager();
	MeshGeometry* mesh = resourceManager->LoadMesh(renderObjectJson["mesh"]);
	TextureBase* tex = resourceManager->LoadTexture(renderObjectJson["texture"]);
	ShaderBase* shader = resourceManager->LoadShader(renderObjectJson["vertexShader"],renderObjectJson["fragmentShader"]);

	gameObject->SetRenderObject(new RenderObject(gameObject->GetTransform(), mesh, tex, shader));
}

void SetPhysicsObjectFromJson(GameObject* gameObject, json physicsObjectJson)
{
	PhysicsObject* po = new PhysicsObject(&gameObject->GetTransform(), gameObject->GetBoundingVolume());
	po->SetInverseMass(1 / physicsObjectJson["mass"]);

}

GameObject* CreateObjectFromJson(json objectJson, Game* game)
{
	GameObject* go = new GameObject(objectJson["name"]);
	Transform& transform = go->GetTransform();
	SetTransformFromJson(transform, objectJson["transform"]);

	if		(objectJson["collider"] == "AABB")		go->SetBoundingVolume(new AABBVolume(transform.GetScale()));
	else if (objectJson["collider"] == "OBB")		go->SetBoundingVolume(new OBBVolume(transform.GetScale()));
	else if (objectJson["collider"] == "Sphere")	go->SetBoundingVolume(new SphereVolume(transform.GetScale().x));

	SetRenderObjectFromJson(go, objectJson["renderObject"], game);

	go->SetPhysicsObject(new PhysicsObject(&transform, go->GetBoundingVolume()));

//	floor->GetPhysicsObject()->SetInverseMass(0);
//	floor->GetPhysicsObject()->InitCubeInertia();
//
//	floor->SetIsStatic(true);
//
//	world->AddGameObject(floor);


	return nullptr;
}


void JsonReader::ReadLevelFromJson(std::string fileName, Game* game)
{
	std::ifstream input{ fileName };

	json level {};

	input >> level;

	for (auto obj : level["objects"])
	{
		game->AddGameObject(CreateObjectFromJson(obj));
	}


}
