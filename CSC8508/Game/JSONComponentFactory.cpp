#include "JsonComponentFactory.h"
#include "JSONShared.h"

#include "MoveComponent.h"
#include "PlayerComponent.h"
#include "RespawnComponent.h"
#include"PlaySound.h"
#include"SetListener.h"
#include "RingComponenet.h"
#include "TeleporterComponent.h"

#include "../Engine/GameObject.h"
#include "Game.h"
#include "BonusComponent.h"
#include "GameStateManagerComponent.h"
#include"DisappearingPlatformComponent.h"
#include"TimeScoreComponent.h"

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
	if (name == "GameStateManager")		return gameObject->AddComponent<GameStateManagerComponent>(game->GetWorld());
	if (name == "ScoreComponent")		return gameObject->AddComponent<ScoreComponent>();
	

	if (name == "PlaySound")	return gameObject->AddComponent<PlaySound>(componentJson["path"], componentJson["mode"], componentJson["volume"], componentJson["min"]);
	if (name == "SetListener")	return gameObject->AddComponent<SetListener>(componentJson["ID"]);

	if (name == "Teleporter")	return gameObject->AddComponent<TeleporterComponent>(JsonToVector3(componentJson["target"]));

	if (name == "Ring")			return gameObject->AddComponent<RingComponent>(10);
	if (name == "DisappearingPlatform")			return gameObject->AddComponent<DisappearingPlatformComponent>();
	if (name == "TimeScoreComponent")			return gameObject->AddComponent<TimeScoreComponent>();

	return nullptr;
}