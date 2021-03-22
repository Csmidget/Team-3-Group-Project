#include "JsonComponentFactory.h"
#include "JSONShared.h"

#include "PlayerComponent.h"
#include "RespawnComponent.h"
#include"PlaySound.h"
#include"SetListener.h"
#include "RingComponent.h"
#include "TeleporterComponent.h"

#include "../Engine/GameObject.h"
#include "Game.h"
#include "BonusComponent.h"
#include "GameStateManagerComponent.h"
#include"DisappearingPlatformComponent.h"
#include"TimeScoreComponent.h"
#include"HingeComponent.h"

using namespace NCL;
using namespace CSC8508;
using namespace JSONShared;

using json = nlohmann::json;

Component* JSONComponentFactory::AddComponentFromJson(json componentJson, GameObject* gameObject, Game* game) {
	
	std::string name = componentJson["name"];

	//if (name == "Bonus")		return gameObject->AddComponent<BonusComponent>(componentJson["reward"]);
	if (name == "DisappearingPlatform")			return gameObject->AddComponent<DisappearingPlatformComponent>();
	if (name == "GameStateManager")		return gameObject->AddComponent<GameStateManagerComponent>(game->GetWorld());
	if (name == "HingeComponent")		return gameObject->AddComponent<HingeComponent>(game, JsonToVector3(componentJson["point"]), JsonToVector3(componentJson["axis"]));
	if (name == "Player")		return gameObject->AddComponent<PlayerComponent>(game);
	if (name == "PlaySound")	return gameObject->AddComponent<PlaySound>(componentJson["path"], componentJson["mode"], componentJson["volume"], componentJson["min"]);
	if (name == "Respawn")		return gameObject->AddComponent<RespawnComponent>();
	if (name == "Ring")			return gameObject->AddComponent<RingComponent>(10);
	if (name == "ScoreComponent")		return gameObject->AddComponent<ScoreComponent>();
	if (name == "SetListener")	return gameObject->AddComponent<SetListener>(componentJson["ID"]);
	if (name == "Teleporter")	return gameObject->AddComponent<TeleporterComponent>(JsonToVector3(componentJson["target"]));
	if (name == "TimeScoreComponent")			return gameObject->AddComponent<TimeScoreComponent>();
	




	return nullptr;
}