#pragma once
#include "BehaviourNodeWithChildren.h"

namespace NCL {
	namespace CSC8508 {
		
		class BehaviourParallel : public BehaviourNodeWithChildren {
		public:
			BehaviourParallel(const std::string& nodeName) : BehaviourNodeWithChildren(nodeName) {};
			~BehaviourParallel() {};

			BehaviourState Execute(float dt) override {
				currentState = BehaviourState::Failure;
				for (auto& i : childNodes) {
					BehaviourState nodeState = i->Execute(dt);
					switch (nodeState) {
						case BehaviourState::Failure: continue;
						case BehaviourState::Ongoing:
							currentState = nodeState;
						case BehaviourState::Success:
						{
							if (currentState != BehaviourState::Ongoing)
								currentState = nodeState;
						}
					}
				}
				return currentState;
			}
		};
	}
}