#pragma once
#include <string>

namespace NCL {
	namespace CSC8508 {

		class Game;

		static class JsonReader {

		public:
			static void ReadLevelFromJson(std::string fileName, Game* game);
		};

	}
}
