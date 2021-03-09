#pragma once
#include "GameObject.h"

namespace NCL {
	namespace CSC8508 {

		class TraversableObject : public GameObject {

		public:
			TraversableObject(std::string name = "", char type = '.') :GameObject(name) {
				traversalType = type;
				tags.insert("traversable");
			}

			char TraversalType() const { return traversalType; }

		private:
			char traversalType;
		};
	}
}