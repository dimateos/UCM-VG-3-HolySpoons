//Nap_Time_Studios
#ifndef GAMESTATEMACHINE_H_
#define GAMESTATEMACHINE_H_

#include "GameState.h" //not worth fowarding
#include <stack>
using namespace std;

typedef union SDL_Event;

//at some global cfg
#define mainGameStateName "MainScene"
#define mainGameStateJSON "_MAIN_LEVEL_"
#define introStateJSON "_MENU_LEVEL_"

class GameStateMachine
{
private:
	stack<GameState*> states_;
	GameStateMachine();
	~GameStateMachine();
	static GameStateMachine* instance_;

	void preStateChanged();
	void postStateChanged();

public:
	static GameStateMachine* getSingleton();
	static void shutdownSingleton();
	GameState* loadLevel(std::string level);

	bool handleEvents(const SDL_Event evt);
	void update(double time);

	//virtual void receive(Message* msg);
	GameState* const currentState ();
	inline int stackSize() const { return states_.size(); }

	void pushState(GameState* pState);
	void popToState(std::string targetName);
	void popState();
	void clearStates();
	void clearToState(GameState* pState);
	bool const stackIsEmpty() const;
};

#endif /* GAMESTATEMACHINE_H_ */