#include "JsonComponentFactory.h"
#include "JSONShared.h"

#include "MoveComponent.h"
#include "PlayerComponent.h"
#include "RespawnComponent.h"
#include"PlaySound.h"
#include"SetListener.h"

#include "../Engine/GameObject.h"
#include "BonusComponent.h"

using namespace NCL;
using namespace CSC8508;
using namespace JSONShared;

using json = nlohmann::json;

Component* JSONComponentFactory::AddComponentFromJson(json componentJson, GameObject* gameObject, Game* game) {
	
	std::string name = componentJson["name"];

	if (name == "Move")			return gameObject->AddComponent<MoveComponent>(JsonToVector3(componentJson["force"]), componentJson["time"]);
	if (name == "Player")		return gameObject->AddComponent<PlayerComponent>(game);
	if (name == "Respawn")		return gameObject->AddComponent<RespawnComponent>();
	if (name == "Bonus")		return gameObject->AddComponent<BonusComponent>(componentJson["reward"]);
	if (name == "PlaySound")	return gameObject->AddComponent<PlaySound>(componentJson["path"], componentJson["mode"], componentJson["volume"]);
	if (name == "SetListener")	return gameObject->AddComponent<SetListener>(componentJson["ID"]);


	return nullptr;
}