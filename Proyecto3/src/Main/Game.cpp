#include "Game.h"
#include "Windows.h" //temp counter method

#include "LogSystem.h"
#include "RenderSystemInterface.h"

#include "TestComponent.h"
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "OverlayComponent.h"
#include "FPSCamera.h"

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

	//Get the singleton instances
	physicsManager = PhysicsSystemManager::getSingleton();
	renderManager = RenderSystemManager::getSingleton();
	//soundManager_ = new SoundManager(this);

	//Config systems
	RenderSystemInterface::createSingleton(renderManager->getSceneManager());

	//Atm everything is in the jsons
	gsm_ = new GameStateMachine(); //!temporary direct creation
	auto level = gsm_->loadLevel("_TEST_LEVEL_"); //gsm uses the parser + factory
	gsm_->pushState(level); //you can push it already and add more things later

	/*
	//proof of concept:
	// * reading the scene from json with prefabs and all
	// * also manually adding the previous tester GO

	//can be created and not added -> doesnt setup
	auto tester1 = new GameObject(nap_json({ { "id", { {"name", "test_gameObject"}, } }, }),
		{ new TestComponent(), new FPSCamera(nap_json({ {"id", {} } })) }
	);

	//can be added afterwards and setsup
	auto tester2 = new GameObject(nap_json({ { "id", { {"name", "test_gameObject"}, } }, }),
		{ new TestComponent(), new FPSCamera(nap_json({ {"id", {} } })) }
	);
	//watch out you need a FPS cam to have a properly placed camera (to look at the bodies etc)
	level->addGameObject(tester2);

	//OverlayTest
	nap_json cfg_overlay_test_rend = {
		{ "id", { {"name", "overlay_test_rend"}, } },
		{"overlay_name", "TEST_HUD"},
		{"panel_container", "TEST_HUD_PanelContainer"}
	};
	nap_json cfg_crosshair_hud_rend = {
		{ "id", { {"name", "crosshair_hud_rend"}, } },
		{ "overlay_name", "CROSSHAIR_HUD" },
		{ "panel_container", "CROSSHAIR_HUD_PanelContainer" }
	};

	//you can add components to already pushed GO and they setup
	tester2->addComponent(new OverlayComponent(cfg_crosshair_hud_rend));
	tester2->addComponent(new OverlayComponent(cfg_overlay_test_rend));
	*/

	LogSystem::Log("initialized", LogSystem::GAME);
}

//destroys the gameStateMachine
void Game::closeGame() {
	LogSystem::Log("closing...", LogSystem::GAME);

	//Close singleton instances
	PhysicsSystemManager::shutdownSingleton();
	renderManager->shutdown(); //maybe static too?

	delete gsm_;
	//delete soundManager_;

	LogSystem::Log("closed", LogSystem::GAME);
}

///////////////////////////////////////////////////////////////////

void Game::start() {
	LogSystem::Log("start game", LogSystem::GAME);
	StartCounter();
	run();
}

void Game::stop() {
	LogSystem::Log("stop game", LogSystem::GAME);
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