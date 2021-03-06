#define NOMINMAX
#include "ScoreComponent.h"
#include "RingComponent.h"
#include "GameTechRenderer.h"
#include "Game.h"
#include "../Engine/GameWorld.h"
#include "NetworkPlayerComponent.h"

#include <algorithm>


using namespace NCL;
using namespace CSC8508;

ScoreComponent* ScoreComponent::instance = nullptr;

void ScoreComponent::DisplayScoreboard(Game* game) {

	typedef std::pair<std::string, int> ScorePair;

	std::vector<ScorePair> playerScores;

	playerScores.push_back({ "Your score", ScoreComponent::instance->GetScore() });

	auto networkPlayers = game->GetWorld()->GetComponentsOfType<NetworkPlayerComponent>();

	for (auto i = 0; i < networkPlayers.size(); i++)
	{
		playerScores.push_back({ "Player " + std::to_string(i), networkPlayers[i]->GetScore() });
	}

	std::sort(playerScores.begin(), playerScores.end(), [](ScorePair a, ScorePair b) {return a.second > b.second; });

	game->getRenderer()->DrawString("Scoreboard:", Vector2(1, 30), Vector4(1.0f,1.0f,1.0f,1.0f), 15.0f);

	for (int i = 0; i < playerScores.size(); ++i)
	{
		string name = playerScores[i].first;
		int score = playerScores[i].second;

		game->getRenderer()->DrawString(name + ": " + std::to_string(score), Vector2(1, 34 + (i * 4)), Vector4(1.0f,1.0f,1.0f,1.0f), 15.0f);
	}


}

ScoreComponent::ScoreComponent(GameObject* object) : Component("ScoreComponent", object)
{
	if (instance)
		gameObject->Destroy();
	else
		instance = this;

	gameObject->SetPersistence(true);

	hasFinished = false;
	score = 0;
}

ScoreComponent::~ScoreComponent()
{
	if (instance = this)
		instance = nullptr;
}

void ScoreComponent::AddScore(int val)
{
	this->score = std::max(0, (score + val));
}
