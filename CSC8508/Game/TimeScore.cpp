#include"TimeScore.h"

NCL::CSC8508::TimeScoreComponent::TimeScoreComponent(GameObject* object) : Component("TimeScoreComponent", object)
{
	timeScore = 0;
}

void NCL::CSC8508::TimeScoreComponent::UpdateScore(float dt)
{
	std::cout << "时间得分";
	getTime.Tick();

	if (num == 1) {
		nowTime = getTime.GetTotalTimeSeconds();
		num = 0;
	}
	else {
		nextTime = getTime.GetTotalTimeSeconds();
		if (nextTime - nowTime == 1) {
			timeScore += 10;
			num = 1;
		}
		else if (nextTime - nowTime > 1) {
			num = 1;
		}
		else {
			num = 0;
		}
	}

	Debug::Print("Your socres is: " + std::to_string(timeScore), Vector2(5, 90));
}
