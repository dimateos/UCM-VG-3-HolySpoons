#include "Game.h"

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

//main game loop, ends with exit_
void Game::run() {
	exit_ = false;

	while (!exit_) {
		cout << endl << "start loop" << endl;
		float start_time = 0; //something

		//STEP PHYSICS
		cout << endl << "\t STEP PHYSICS" << endl;
		//retrieve active actors -> update ogre nodes transforms
			//retrieve collisions (add to events queue?)

		//EVENTS
		cout << endl << "\t EVENTS" << endl;
		//handleCollisions(start_time); //if no events queue?
		handleEvents(start_time); //atm sets exit

		//LOGIC
		cout << endl << "\t LOGIC" << endl;
		gsm_->update(start_time);

		//RENDER OGRE
		cout << endl << "\t RENDER OGRE" << endl;

		//fixed time
		//float end_time = get_Time() - start_time;
		//if (end_time < _MS_BETWEEN_FRAMES_)
		//	Delay(_MS_BETWEEN_FRAMES_ - end_time);
	}
}

void Game::start() {
	cout << endl << "start game" << endl;
	run();
}

void Game::stop() {
	cout << endl << "stop game" << endl;
	exit_ = true;
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