#pragma once

#include "..\Engine\GameObject.h"

namespace NCL {
	namespace CSC8508 {
		class StateMachine;
		class StateGameObject : public GameObject {
		public:
			StateGameObject();
			~StateGameObject();

			void MoveLeft(float dt);
			void MoveRight(float dt);

			virtual void Update(float dt) override;
		protected:
			StateMachine* stateMachine;
			float counter;
		};
	}
}