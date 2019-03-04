#include "Game.h"
#include "Windows.h" //temp counter method

Game::Game() {
	initGame();
}

Game::~Game() {
	closeGame();
}

//creates the gameStateMachine, pushes the first state...
void Game::initGame() {
	cout << endl << "initializing game..." << endl;

	//Get the singleton instances
	physicsManager = PhysicsSystemManager::getSingleton();
	renderManager = RenderSystemManager::getSingleton();

	gsm_ = new GameStateMachine(); //!temporary direct creation
	//soundManager_ = new SoundManager(this);
}

//destroys the gameStateMachine
void Game::closeGame() {
	cout << endl << "closing game..." << endl;

	//Close singleton instances
	PhysicsSystemManager::shutdownSingleton();
	renderManager->shutdown(); //maybe static too?

	delete gsm_;
	//delete soundManager_;
}

///////////////////////////////////////////////////////////////////

void Game::start() {
	cout << endl << "start game" << endl;
	StartCounter();
	run();
}

void Game::stop() {
	cout << endl << "stop game" << endl;
	exit_ = true;
}

void Game::StartCounter() {
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return;

	PCFreq = double(li.QuadPart) /*/ 1000.0*/;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	CounterLast = CounterStart;
}

double Game::GetCounter() {
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double t = double(li.QuadPart - CounterLast) / PCFreq;
	CounterLast = li.QuadPart;
	return t;
}

///////////////////////////////////////////////////////////////////

//main game loop, ends with exit_
void Game::run() {
	exit_ = false;

	while (!exit_) {
		//cout << endl << "start loop" << endl;
		double t = GetCounter();

#ifdef FIXED_STEP
		if (t < (1.0f / 30.0f)) {
			fprintf(stderr, "Time: %f\n", stepTime);
			stepTime += t;
		}
		else stepTime = 1.0f / 30.0f;

		if (stepTime >= (1.0f / 30.0f)) {
			t = stepTime;
			stepTime = 0.0f;
		}
#endif
		//STEP PHYSICS
		//cout << endl << "\t STEP PHYSICS" << endl;
		physicsManager->stepPhysics(t);

		//retrieve active actors -> update ogre nodes transforms
			//retrieve collisions (add to events queue?)

		//EVENTS
		//cout << endl << "\t EVENTS" << endl;
		//handleCollisions(start_time); //if no events queue?
		renderManager->pollEvents();
		//handleEvents(t); //atm sets exit

		//LOGIC
		//cout << endl << "\t LOGIC" << endl;
		gsm_->update(t);

		//RENDER OGRE
		//cout << endl << "\t RENDER OGRE" << endl;
		renderManager->renderFrame();
	}
}

//handle main events (Ex. quit) and call the gameStateMachine machine update
void Game::handleEvents(float time) {
	Event evt = UNDEFINED; // pollEvents() some way
	bool propagation = true; //stop global propagation

	//window x botton - close game
	if (evt == GAME_QUIT) {
		propagation = false;
		stop();
	}

	//else if (event.type == SDL_KEYDOWN) {
	//	switch (event.key.keysym.sym) {
	//		//case SDLK_ESCAPE:
	//		//	propagation = false;
	//		//	stop();
	//		//	break;

	//	case SDLK_m:
	//		sendToSM(&Message(VOLUP));
	//		break;
	//	case SDLK_n:
	//		sendToSM(&Message(VOLDOWN));
	//		break;
	//	}
	//}

	if (propagation) gsm_->handleEvents(time, evt);
}