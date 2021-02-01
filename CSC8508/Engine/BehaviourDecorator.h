#pragma once

#include "BehaviourNode.h"
#include <functional>

namespace NCL {
	namespace CSC8508 {
		typedef std::function<BehaviourState(BehaviourState)> BehaviourDecoratorFunc;

		class BehaviourDecorator : public BehaviourNode {
		public:
			BehaviourDecorator(const std::string& nodeName, BehaviourDecoratorFunc f) : BehaviourNode(nodeName) {
				function = f;
				childNode = nullptr;
			}

			~BehaviourDecorator() {
				delete childNode;
			}

			BehaviourState Execute(float dt) override {
				if (childNode) {
					currentState = childNode->Execute(dt);
					currentState = function(currentState);

					return currentState;
				}

				return BehaviourState::Failure;
			}

			void SetChild(BehaviourNode* child) {
				childNode = child;
			}

			void Reset() override {
				currentState = BehaviourState::Initialise;
				childNode->Reset();
			}

		protected:
			BehaviourDecoratorFunc function;
			BehaviourNode* childNode;
		};
	}
}
