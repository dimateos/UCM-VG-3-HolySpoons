#include "Game.h"
#include "Windows.h" //temp counter method

#include "LogSystem.h"
#include "JsonReader.h"
#include "RenderSystemInterface.h"

Game::Game() {
	initGame();
}

Game::~Game() {
	//closed on loop end
}

//creates the gameStateMachine, pushes the first state...
void Game::initGame() {
	//Logging system
	LogSystem::resetLogFile();
	LogSystem::Log("initializing...", LogSystem::GAME);

	//Get/create the singleton instances
	physicsManager = PhysicsSystemManager::getSingleton();
	renderManager = RenderSystemManager::getSingleton();
	soundManager_ = SoundManager::getSingleton();
	messageSystem = MessageSystem::getSingleton();

	//Config systems
	RenderSystemInterface::createSingleton(renderManager->getSceneManager());
	RenderSystemInterface::getSingleton()->setSkyBox("SkyBox");

	//Initialize level
	LogSystem::cls();
	LogSystem::Log("singletons done -> initializing level...", LogSystem::GAME);
	gsm_ = GameStateMachine::getSingleton();
	auto level = gsm_->loadLevel("_TEST_LEVEL_"); //gsm uses the parser + factory
	gsm_->pushState(level); //you can push it already and add more things later

	//all done
	LogSystem::cls();
	LogSystem::Log("initialized", LogSystem::GAME);

}

//shutdown singletons etc in reverse order
void Game::closeGame() {
	LogSystem::Log("closing...", LogSystem::GAME);
	LogSystem::cls();

	gsm_->shutdownSingleton();

	//Close singleton instances
	JsonReader::shutdownSingleton();
	MessageSystem::shutdownSingleton();
	SoundManager::shutdownSingleton();
	renderManager->shutdownSingleton(); //maybe static too?
	PhysicsSystemManager::shutdownSingleton();

	LogSystem::Log("closed", LogSystem::GAME);
}

///////////////////////////////////////////////////////////////////

void Game::start() {
	LogSystem::Log("starting game", LogSystem::GAME);
	run();
}

void Game::stop() {
	LogSystem::Log("stopping game", LogSystem::GAME);
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
	StartCounter();

	while (!exit_) {
		double t = GetCounter();

#ifdef FIXED_STEP
		if (t < (1.0f / 30.0f)) {
			//fprintf(stderr, "Time: %f\n", stepTime);
			stepTime += t;
		}
		else stepTime = 1.0f / 30.0f;

		if (stepTime >= (1.0f / 30.0f)) {
			t = stepTime;
			stepTime = 0.0f;
		}
#endif
		// STEP PHYSICS
		//LogSystem::Log("main physics", LogSystem::GAME);
		physicsManager->stepPhysics(t);
		physicsManager->updateNodes();

		//retrieve collisions (add to events queue? or messages?)

		// EVENTS
		//LogSystem::Log("main handleEvents", LogSystem::GAME);
		handleEvents(); //atm sets exit

		//handleCollisions(start_time); //if no events queue?

		// LOGIC
		//LogSystem::Log("main update", LogSystem::GAME);
		gsm_->update(t); //and its sub-parts like lateUpdate

		// RENDER OGRE
		//LogSystem::Log("main render", LogSystem::GAME);
		renderManager->renderFrame();

		// SOUND
		//LogSystem::Log("main sound", LogSystem::GAME);
		soundManager_->update();
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