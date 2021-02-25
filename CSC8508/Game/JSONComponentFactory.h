#pragma once
#include "../../Plugins/json/json.hpp"

namespace NCL {
	namespace CSC8508 {

		class Game;
		class GameObject;
		class Component;

		namespace JSONComponentFactory {

			 Component* AddComponentFromJson(nlohmann::json componentJson, GameObject* gameObject, Game* game);

		};
	}
}
