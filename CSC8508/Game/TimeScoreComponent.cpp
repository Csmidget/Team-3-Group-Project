#include"TimeScoreComponent.h"

NCL::CSC8508::TimeScoreComponent::TimeScoreComponent(GameObject* object) : Component("TimeScoreComponent", object)
{
	timeScore = 0;
	nowTime = 0;
	nextTime = 0;
	num = 1;
}

int NCL::CSC8508::TimeScoreComponent::UpdateScore(float dt)
{
	getTime.Tick();

	if (num == 1) {
		nowTime = getTime.GetTotalTimeSeconds();
		num = 0;
	}
	else {
		nextTime = getTime.GetTotalTimeSeconds();
		if (nextTime - nowTime == 1) {
			timeScore += 1;
			num = 1;
		}
		else if (nextTime - nowTime > 1) {
			num = 1;
		}
		else {
			num = 0;
		}
		return timeScore;
	}

}
