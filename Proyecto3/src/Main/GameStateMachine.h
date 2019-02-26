//Nap_Time_Studios

#pragma once

#include "GameState.h"
#include <stack>

#include "Listener.h"
//#include "Emitter.h"

class GameStateMachine : public Listener//, public Emitter
{
public:
	GameStateMachine();
	~GameStateMachine();

	void handleEvents(float time, const Event evt);
	void update(float time);

	virtual void receive(Message* msg);

	//GameState* currentState() { return states_.top(); } //unused

private:
	stack<GameState*> states_;

	void pushState(GameState* pState);
	void changeState(GameState* pState);
	void popState();
};
