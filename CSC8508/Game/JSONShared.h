#pragma once
#include "../../Plugins/json/json.hpp"

#include "../../Common/Vector3.h"
#include "../../Common/Quaternion.h"

namespace NCL
{
	namespace CSC8508 {
		
		namespace JSONShared {

			Maths::Vector3 JsonToVector3(nlohmann::json vectorJson);

			Maths::Quaternion JsonToQuaternion(nlohmann::json quatJson);
		}
	}
}