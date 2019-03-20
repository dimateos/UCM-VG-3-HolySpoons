#include "Game.h"
#include "Windows.h" //temp counter method

#include "PhysicsComponent.h"
#include "FPSCamera.h"
#include "RenderSystemInterface.h"

Game::Game() {
	initGame();
}

Game::~Game() {
	//closed on loop end
}

//creates the gameStateMachine, pushes the first state...
void Game::initGame() {
	cout << endl << "initializing game..." << endl;

	//Get the singleton instances
	physicsManager = PhysicsSystemManager::getSingleton();
	renderManager = RenderSystemManager::getSingleton();
	RenderSystemInterface::createSingleton(renderManager->getSceneManager());

	//!temporary direct creation
	gsm_ = new GameStateMachine();
	gsm_->pushState(new GameState({ new GameObject({ new PhysicsComponent(), new FPSCamera() }) }));

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
		physicsManager->updateNodes();
		//retrieve collisions (add to events queue? or messages?)

		//EVENTS
		//cout << endl << "\t EVENTS" << endl;
		//handleCollisions(start_time); //if no events queue?
		handleEvents(); //atm sets exit

		//LOGIC
		//cout << endl << "\t LOGIC" << endl;
		gsm_->update(t);

		//RENDER OGRE
		//cout << endl << "\t RENDER OGRE" << endl;
		renderManager->renderFrame();
	}

	closeGame();
}

//handle main events (Ex. quit) and call the gameStateMachine machine update
void Game::handleEvents() {

	SDL_Event evt;
	while (SDL_PollEvent(&evt)) {
		bool handled = false; //stop global propagation

		//window x botton - close game
		if (evt.type == SDL_QUIT) {
			handled = true;
			stop();
		}
		else if (evt.type == SDL_KEYDOWN) {
			switch (evt.key.keysym.sym) {
				case SDLK_ESCAPE:
					handled = true;
					stop();
					break;
			}
		}

		if (!handled) handled = renderManager->handleEvents(evt);
		if (!handled) handled = gsm_->handleEvents(evt);
	}
}