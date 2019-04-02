//Nap_Time_Studios
#ifndef GAME_H_
#define GAME_H_

#include "RenderSystemManager.h"
#include "PhysicsSystemManager.h"
#include "SoundManager.h"
#include "MessageSystem.h"

#include "GameStateMachine.h"

class Game {
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

	///////////////////////////////////////////////////////////////////
	// based on fixed step by physx example blueprints

	double PCFreq = 0.0;
	__int64 CounterStart = 0;
	__int64 CounterLast = 0;

	void StartCounter();
	double GetCounter();

	float stepTime = 0.0f;
	//#define FIXED_STEP

	///////////////////////////////////////////////////////////////////

	bool exit_; //ends the game

	PhysicsSystemManager* physicsManager;
	RenderSystemManager* renderManager;
	SoundManager* soundManager_;
	MessageSystem* messageSystem;

	GameStateMachine* gsm_; //controls the states
};

#endif /* GAME_H_ */