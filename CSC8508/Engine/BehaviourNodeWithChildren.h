#pragma once

#include "BehaviourNode.h"
#include <vector>

namespace NCL {
	namespace CSC8508 {

		class BehaviourNodeWithChildren : public BehaviourNode {
		public:
			BehaviourNodeWithChildren(const std::string& nodeName) : BehaviourNode(nodeName) {};
			
			~BehaviourNodeWithChildren() {
				for (auto& i : childNodes) {
					delete i;
				}
			}

			void AddChild(BehaviourNode* n) {
				childNodes.emplace_back(n);
			}

			void Reset() override {
				currentState = BehaviourState::Initialise;
				for (auto& i : childNodes) {
					i->Reset();
				}
			}


		protected:
			std::vector<BehaviourNode*> childNodes;
		};
	}
}