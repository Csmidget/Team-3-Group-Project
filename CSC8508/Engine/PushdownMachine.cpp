#include "PushdownMachine.h"
#include "PushdownState.h"

using namespace NCL::CSC8508;

bool PushdownMachine::Update(float dt) {
	if (activeState) {
		PushdownState* newState = nullptr;

		PushdownState::PushdownResult result = activeState->OnUpdate(dt, &newState);

		switch (result) {
			case PushdownState::PushdownResult::Pop: {
				activeState->OnSleep();
				delete activeState;
				stateStack.pop();
				if (stateStack.empty()) {
					return false;
				}
				else {
					activeState = stateStack.top();
					activeState->OnAwake();
				} 
			} break;
			case PushdownState::PushdownResult::Push: {
				activeState->OnSleep();
				stateStack.push(newState);
				activeState = newState;
				activeState->OnAwake();
			} break;
			case PushdownState::PushdownResult::Replace: {
				activeState->OnSleep();
				delete activeState;
				stateStack.pop();
				stateStack.push(newState);
				activeState = newState;
				activeState->OnAwake();
			} break;
		}

	}
	else {
		stateStack.push(initialState);
		activeState = initialState;
		activeState->OnAwake();
	}
	return true;
}