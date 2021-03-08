#pragma once

namespace NCL {
	namespace CSC8508 {

		class PushdownState {
		public:
			enum class PushdownResult {
				Push, Pop, NoChange, Over, Top, Replace
			};
			PushdownState() {};
			virtual ~PushdownState() {};

			virtual PushdownResult OnUpdate(float dt, PushdownState** pushFunc) = 0;
			virtual void OnAwake() {};
			virtual void OnSleep() {};
		};
	}
}