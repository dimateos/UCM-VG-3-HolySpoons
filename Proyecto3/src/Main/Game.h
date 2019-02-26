//Nap_Time_Studios

#pragma once

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
	void handleEvents(float time);

	///////////////////////////////////////////////////////////////////

	bool exit_; //ends the game

	GameStateMachine* gsm_; //controls the states
	//SoundManager* soundManager_; //controls the sound
};