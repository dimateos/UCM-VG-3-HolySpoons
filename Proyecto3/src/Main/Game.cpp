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
	cout << endl << "initializing game..." << endl;

	//Get the singleton instances
	physicsManager = PhysicsSystemManager::getSingleton();
	renderManager = RenderSystemManager::getSingleton();
	RenderSystemInterface::createSingleton(renderManager->getSceneManager());
	//soundManager_ = new SoundManager(this);

	//!temporary direct creation
	gsm_ = new GameStateMachine();

	//proof of concept:
	// * two GO cfg by json (with render + physx cfg by json)
	// * tester GO (with testComponent and test names)

	nap_json cfg_ground = {
		{"name", "ground"},
		{"pos", {
			{"x", 0.0f}, {"y", -15.0f}, {"z", 0.0f}
		}},
		{"ori", {
			{"w", 1.0f}, {"x", 0.0f}, {"y", 0.0f}, {"z", 0.0f}
		}}
	};
	nap_json cfg_ground_phys = {
		{"name", "ground_phys"},
		{"dynamic", false },
		{"shape", {
			{"type", "BOX"},
			{"x", 10.0f}, {"y", 1.0f}, {"z", 10.0f}
		}},
	};
	nap_json cfg_ground_rend = {
		{"name", "ground_rend"},
		{"scale", {
			{"x", 10.0f}, {"y", 1.0f}, {"z", 10.0f}
		}},
		{"shape", {
			{"name", "ground"},
			{"type", "BOX"},
			{"mesh", "cube.mesh"}
		}},
		{"material", "DebugMaterial2"}
	};
	auto phys_ground = new PhysicsComponent(cfg_ground_phys); //physic component is a listner
	auto ground = new GameObject(cfg_ground, { phys_ground, new RenderComponent(cfg_ground_rend) }, { phys_ground });

	nap_json cfg_cube = {
		{"name", "cube"},
		{"pos", {
			{"x", 0.0f}, {"y", 5.0f}, {"z", 0.0f}
		}},
		{"ori", {
			{"w", 1.0f}, {"x", 0.0f}, {"y", 0.0f}, {"z", 0.0f}
		}}
	};
	nap_json cfg_cube_phys = {
	{"name", "cube_phys"},
	{"dynamic", true },
	{"shape", {
		{"type", "BOX"},
		{"x", 1.0f}, {"y", 1.0f}, {"z", 1.0f}
	}},
	};
	nap_json cfg_cube_rend = {
		{"name", "cube_rend"},
		{"namess", ""},
		{"scale", {
			{"x", 1.0f}, {"y", 1.0f}, {"z", 1.0f}
		}},
		{"shape", {
			{"name", "cube"},
			{"type", "BOX"},
			{"mesh", "cube.mesh"}
		}},
		{"material", "DebugMaterial2"}
	};

	//OverlayTest
	nap_json cfg_overlay_test_rend = {
		{ "name", "overlay_test_rend" },
		{ "namess", "" },
		{"overlay_name", "TEST_HUD"},
		{"panel_container", "TEST_HUD_PanelContainer"}
	};

	nap_json cfg_crosshair_hud_rend = {
		{ "name", "crosshair_hud_rend" },
		{ "namess", "" },
		{ "overlay_name", "CROSSHAIR_HUD" },
		{ "panel_container", "CROSSHAIR_HUD_PanelContainer" }
	};

	auto phys_cube = new PhysicsComponent(cfg_cube_phys); //physic component is a listner
	auto cube = new GameObject(cfg_cube, { phys_cube, new RenderComponent(cfg_cube_rend) }, { phys_cube });

	GameObject* tester = new GameObject(nap_json({ {"name", "test_gameObject"} }), { new TestComponent(),
		new FPSCamera(nap_json({ {"name", "fpsCam"} })), new OverlayComponent(cfg_crosshair_hud_rend) });

	GameObject* testOverlay = new GameObject(nap_json({ "name", "test_overlay" }), {new OverlayComponent(cfg_overlay_test_rend)});

	gsm_->pushState(new GameState({ ground, cube, tester }));
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
		//STEP PHYSICS
		physicsManager->stepPhysics(t);
		physicsManager->updateNodes();
		//retrieve collisions (add to events queue? or messages?)

		//EVENTS
		//handleCollisions(start_time); //if no events queue?
		handleEvents(); //atm sets exit

		//LOGIC
		gsm_->update(t); //and its sub-parts like lateUpdate

		//RENDER OGRE
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