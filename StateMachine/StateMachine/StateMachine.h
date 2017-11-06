#pragma once
#include "Header.h"
#include "BaseState.h"
#include <functional>
#include <unordered_map>
#include <memory>
#include "Logger.h"

using namespace std;
using DataType = shared_ptr<Data>;
template <class StateType, class EnventType, class KEY = string>
class StateMachine
{
public:
	using Handler = function<bool(DataType data)>;
	StateMachine() {};
	bool HanleEvent(KEY key, EnventType event, DataType data = nullptr) {
		if (m_StateMachineHandler.find(key) == m_StateMachineHandler.end())
		{
			// Start from start node
			m_StateMachineHandler[key] = m_StartState;
		}
		auto& currentState = m_StateMachineHandler[key];
		auto itrStart = m_mapGraph.find(currentState);
		if (itrStart == m_mapGraph.end())
		{
			LOG("State is not registered");
			return false;
		}
		auto startNode = itrStart->second;
		auto itr = startNode.find(event);
		if (itr != startNode.end())
		{
			LOG("Edge is existed for start node");
			return false;
		}
		auto& handler = itr->second;

		auto& start = m_mapStateStore[currentState];
		auto& end = m_mapStateStore[handler.m_end];

		if (!start->PostState())
		{
			return false;
		}
		if (handler.m_Handler && !handler.m_Handler(data))
		{
			return false;
		}
		if (!end->PreState())
		{
			return false;
		}
		currentState = handler.m_end;
		return true;

	}
	bool Connect(StateType start, StateType end, EnventType event, Handler func = nullptr)
	{
		auto itrStart = m_mapGraph.find(start);
		if (itrStart == m_mapGraph.end())
		{
			LOG("State is not registered");
			return false;
		}

		auto startNode = itrStart->second;
		auto itr = startNode.find(event);
		if (itr != startNode.end())
		{
			LOG("edge is existed for start node");
			return false;
		}

		startNode[event] = { end, func };
		return true;
	}
	bool AddState(StateType stateType) {
		auto state = make_shared<State<StateType>>(stateType);
		return AddBaseState(state);
	}

	bool AddStart(StateType stateType) {
		m_StartState = stateType;
		return AddState(stateType);
	}

	template<typename TYPE>
	bool AddState() {
		auto state = make_shared<TYPE>();
		return AddBaseState(state);
	}

	bool AddBaseState(shared_ptr<BaseState<StateType>> state) {
		auto stateType = state->GetState();
		if (m_mapGraph.find(stateType) != m_mapGraph.end())
		{
			LOG("State is added before");
			return false;
		}
		m_mapGraph[stateType] = unordered_map<Event, StateFunctor>();
		m_mapStateStore[stateType] = state;
		return true;
	}
	~StateMachine() {};
private:
	struct StateFunctor {
		StateType m_end;
		Handler m_Handler;
	};
	unordered_map<StateType, unordered_map<EnventType, StateFunctor>> m_mapGraph;
	unordered_map<StateType, shared_ptr<BaseState<StateType>> > m_mapStateStore;
	unordered_map<KEY, StateType> m_StateMachineHandler;
	StateType m_StartState;
};

