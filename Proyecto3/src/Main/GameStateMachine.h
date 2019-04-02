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
	void update(double time);

	//virtual void receive(Message* msg);
	GameState* const currentState ();

	void pushState(GameState* pState);
	void changeState(GameState* pState);
	void popState();
	void clearStates();


private:
	stack<GameState*> states_;
};

#endif /* GAMESTATEMACHINE_H_ */