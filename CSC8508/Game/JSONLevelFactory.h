#pragma once
#include <string>

namespace NCL {
	namespace CSC8508 {

		class Game;

		namespace JSONLevelFactory {
			void ReadLevelFromJson(std::string fileName, Game* game);
		}
	}
}
