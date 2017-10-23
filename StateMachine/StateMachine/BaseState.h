#pragma once

template <class StateType>
class BaseState
{
public:
	BaseState(const StateType& state) : m_State(state) {};
	~BaseState() {};
	virtual bool PreState() = 0;
	virtual bool PostState() = 0;
	StateType GetState() {
		return m_State;
	};
protected:
	const StateType m_State;
};

