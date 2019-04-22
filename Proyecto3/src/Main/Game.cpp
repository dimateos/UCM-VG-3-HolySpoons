#include "Game.h"

//fowarded
#include <LogSystem.h>
#include <JsonReader.h>
#include <GameStateMachine.h>
#include <MessageSystem.h>
#include <RenderSystemManager.h>
#include <RenderSystemInterface.h>
#include <PhysicsSystemManager.h>
#include <SoundManager.h>

#include <SDL_events.h>	//events
#include <TimeSystem.h>	//timing

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
	LogSystem::Log("initializing...", LogSystem::GAME, {__FILE__, __LINE__});

	//Get/create the singleton instances
	physicsManager_ = PhysicsSystemManager::getSingleton();
	renderManager_ = RenderSystemManager::getSingleton();
	soundManager_ = SoundManager::getSingleton();
	messageSystem_ = MessageSystem::getSingleton();

	//Config rendering scenes
	renderManager_->setupScene("Menu"); //creates the first scene
	renderManager_->setupScene("MainScene");
	renderManager_->setupScene("Pause");
	RenderSystemInterface::getSingleton()->setRenderingScene("MainScene");	//sets rendering scene
	RenderSystemInterface::getSingleton()->setSkyBox("SkyBox2");			//may be an object?
	RenderSystemInterface::getSingleton()->setRenderingScene("Menu");		//sets rendering scene

	//Initialize level
	JsonReader::getSingleton(); //load prefabs
	LogSystem::cls();
	LogSystem::Log("singletons done -> initializing level...", LogSystem::GAME);

	gsm_ = GameStateMachine::getSingleton();
	auto menu = gsm_->loadLevel(introStateJSON); //gsm uses the parser + factory
	gsm_->pushState(menu); //you can push it already and add more things later

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
	RenderSystemInterface::shutdownSingleton();
	RenderSystemManager::shutdownSingleton();
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

///////////////////////////////////////////////////////////////////

//main game loop, ends with exit_
void Game::run() {
	exit_ = false;

#ifndef FIXED_STEP
	TimeSystem::StartCounter();
#endif

	while (!exit_) {
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
#else
		double t = TimeSystem::GetCounter();
#endif
		//LogSystem::Log("time step: ", t, LogSystem::GAME);

		// STEP PHYSICS
		//LogSystem::Log("main physics", LogSystem::GAME);
		physicsManager_->stepPhysics(t);
		physicsManager_->updateNodes();

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
		renderManager_->renderFrame();

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
				case SDLK_9:
					RenderSystemInterface::getSingleton()->setRenderingScene("MainScene");
					GameStateMachine::getSingleton()->popState();
					break;
			}
		}

		if (!handled) handled = renderManager_->handleEvents(evt);
		if (!handled) handled = gsm_->handleEvents(evt);
	}
}