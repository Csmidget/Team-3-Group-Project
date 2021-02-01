#pragma once

#include "BehaviourNodeWithChildren.h"

namespace NCL {
	namespace CSC8508 {
		class BehaviourSelector : public BehaviourNodeWithChildren {
		public:
			BehaviourSelector(const std::string& nodeName) : BehaviourNodeWithChildren(nodeName) {};
			~BehaviourSelector() {};

			BehaviourState Execute(float dt) override {
				for (auto& i : childNodes) {
					BehaviourState nodeState = i->Execute(dt);
					switch (nodeState) {
						case BehaviourState::Failure: continue;
						case BehaviourState::Success:
						case BehaviourState::Ongoing:
						{
							currentState = nodeState;
							return currentState;
						}
					}
				}
				return BehaviourState::Failure;
			}
		};

	}
}