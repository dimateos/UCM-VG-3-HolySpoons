//Nap_Time_Studios
#pragma once

#include "RenderSystemManager.h"
#include "PhysicsSystemManager.h"
#include "GameStateMachine.h"
//#include "SoundManager.h"

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
	GameStateMachine* gsm_; //controls the states
	//SoundManager* soundManager_; //controls the sound
};