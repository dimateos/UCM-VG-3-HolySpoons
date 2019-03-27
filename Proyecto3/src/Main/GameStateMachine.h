//Nap_Time_Studios
#ifndef GAMESTATEMACHINE_H_
#define GAMESTATEMACHINE_H_

#include "GameState.h"
#include <stack>

//#include "Listener.h"
class GameStateMachine// : public Listener
{
public:
	GameStateMachine();
	~GameStateMachine();

	GameState* loadLevel(std::string level);

	bool handleEvents(const SDL_Event evt);
	void update(float time);

	//virtual void receive(Message* msg);
	//GameState* currentState() { return states_.top(); } //unused

	void pushState(GameState* pState);
	void changeState(GameState* pState);
	void popState();

private:
	stack<GameState*> states_;
};

#endif /* GAMESTATEMACHINE_H_ */