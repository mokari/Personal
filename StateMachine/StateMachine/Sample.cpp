#include "StateMachine.h"
#include "State.h"
#include "Header.h"


enum class Event {
	E_FINISH,
	E_GO
};

enum class States {
	S_Init,
	S_Mid,
	S_Halt
};

class MiddleState : public State<States> {
public:
	MiddleState() :State(States::S_Mid) {};
	bool PreState() override {
		LOG("Enter to Mid");
		return true;
	};
	bool PostState() override {
		LOG("Exit from Mid");
		return true;
	};
};

int main()
{
	StateMachine<States, Event> machine;
	machine.AddStart(States::S_Init);
	machine.AddState(States::S_Halt);
	machine.AddState<MiddleState>();
	machine.Connect(States::S_Init, States::S_Mid, Event::E_GO, [](auto data) { LOG("Init to Mid, Go event"); return true; });
	machine.Connect(States::S_Init, States::S_Halt, Event::E_FINISH);
	machine.Connect(States::S_Mid, States::S_Halt, Event::E_GO);

	machine.HanleEvent("", Event::E_GO);
	machine.HanleEvent("", Event::E_GO);

	return 0;
}