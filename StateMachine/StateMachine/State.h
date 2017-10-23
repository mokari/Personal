#pragma once
#include "BaseState.h"
#include "Logger.h"
template <class StateType>
class State :
	public BaseState<StateType>
{
public:
	State(StateType state) :BaseState(state) {
	};
	~State() {};
	bool PreState() override {
		LOG("Enter to: %d", this->GetState());
		return true;
	};
	bool PostState() override {
		LOG("Exit from: %d",
			this->GetState());
		return true;
	};
};

