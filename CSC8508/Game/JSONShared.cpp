#include "JSONShared.h"

using namespace NCL;
using namespace CSC8508;


Maths::Vector3 JSONShared::JsonToVector3(nlohmann::json vectorJson)
{
	return { vectorJson["x"],vectorJson["y"],vectorJson["z"] };
}

Maths::Quaternion JSONShared::JsonToQuaternion(nlohmann::json quatJson)
{
	return { quatJson["x"],quatJson["y"],quatJson["z"],quatJson["w"] };
}