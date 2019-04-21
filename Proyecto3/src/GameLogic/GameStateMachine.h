//Nap_Time_Studios
#ifndef GAMESTATEMACHINE_H_
#define GAMESTATEMACHINE_H_

#include "GameState.h" //not worth fowarding
#include <stack>
using namespace std;

typedef union SDL_Event;

//at some global cfg
#define mainGameState "MainScene"
#define mainGameStateJSON "_MAIN_LEVEL_"
#define introStateJSON "_MENU_LEVEL_"

class GameStateMachine
{
private:
	stack<GameState*> states_;
	GameStateMachine();
	~GameStateMachine();
	static GameStateMachine* instance_;

public:
	static GameStateMachine* getSingleton();
	static void shutdownSingleton();
	GameState* loadLevel(std::string level);

	bool handleEvents(const SDL_Event evt);
	void update(double time);

	//virtual void receive(Message* msg);
	GameState* const currentState ();

	void pushState(GameState* pState);
	void changeState(GameState* pState);
	void popState();
	void clearStates();
};

#endif /* GAMESTATEMACHINE_H_ */