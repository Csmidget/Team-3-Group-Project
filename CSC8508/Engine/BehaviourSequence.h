#pragma once
#include "BehaviourNodeWithChildren.h"

namespace NCL {
	namespace CSC8508 {
		
		class BehaviourSequence : public BehaviourNodeWithChildren {
		public:
			BehaviourSequence(const std::string& nodeName) : BehaviourNodeWithChildren(nodeName) {};
			~BehaviourSequence() {};

			BehaviourState Execute(float dt) override {
				for (auto& i : childNodes) {
					BehaviourState nodeState = i->Execute(dt);
					switch (nodeState) {
						case BehaviourState::Success: continue;
						case BehaviourState::Failure:
						case BehaviourState::Ongoing:
						{
							currentState = nodeState;
							return nodeState;
						}
					}
				}
				return BehaviourState::Success;
			}

		};
	}
}