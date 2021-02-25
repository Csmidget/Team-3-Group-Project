#include "JsonComponentFactory.h"
#include "JSONShared.h"

#include "MoveLeftComponent.h"
#include "MoveRightComponent.h"
#include "MoveComponent.h"
#include "../Engine/GameObject.h"

using namespace NCL;
using namespace CSC8508;
using namespace JSONShared;

using json = nlohmann::json;

Component* JSONComponentFactory::AddComponentFromJson(json componentJson, GameObject* gameObject, Game* game) {
	
	std::string name = componentJson["name"];

	if (name == "Move") return gameObject->AddComponent<MoveComponent>(JsonToVector3(componentJson["force"]));
	if (name == "MoveLeft") return gameObject->AddComponent<MoveLeftComponent>();
	if (name == "MoveRight") return gameObject->AddComponent<MoveRightComponent>();

	return nullptr;
}