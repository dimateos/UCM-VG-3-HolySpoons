//Nap_Time_Studios
#include "checkML.h" //for memory leaks
#ifndef GAME_H_
#define GAME_H_

//forwarding
class GameStateMachine;
class MessageSystem;
class PhysicsSystemManager;
class RenderSystemManager;
class SoundManager;
//main game class: inits / shuts down the app, handles the loop, etc
class Game
{
public:
	Game();
	~Game();

	void start(); //starts the game (run)
	void stop(); //stops the game (end)

private:
	void initGame();
	void closeGame();

	void run();
	void handleEvents();

#undef FIXED_STEP
#ifdef FIXED_STEP
	float stepTime = 0.0f;
#endif

	bool exit_; //ends the game

	GameStateMachine* gsm_; //controls the states
	MessageSystem* messageSystem_;
	PhysicsSystemManager* physicsManager_;
	RenderSystemManager* renderManager_;
	SoundManager* soundManager_;
};

#endif /* GAME_H_ */