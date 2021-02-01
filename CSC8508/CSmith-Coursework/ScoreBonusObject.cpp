#include "ScoreBonusObject.h"
#include "ActorObject.h"

using namespace NCL::CSC8508;

ScoreBonusObject::ScoreBonusObject(Vector3 respawnPosition, int score, float respawnDelay) : BonusObject(respawnPosition, respawnDelay) {	
	this->score = score;
}

void ScoreBonusObject::ApplyBonus(ActorObject* otherObject) {
	otherObject->AddScore(score);
}