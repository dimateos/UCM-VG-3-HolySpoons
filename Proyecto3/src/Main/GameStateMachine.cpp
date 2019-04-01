#include "GameStateMachine.h"

GameStateMachine::GameStateMachine() {}

GameStateMachine::~GameStateMachine() {
	while (!states_.empty()) {
		delete states_.top(); //delete its states
		states_.pop();
	}
}

//void GameStateMachine::receive(Message* msg) {
//	switch (msg->id_) {
//	default:
//		break;
//		//msg recieved
//	}
//}

#include "ComponentFactory.h" //build comps
#include "JsonReader.h" //reading levels

GameState * GameStateMachine::loadLevel(std::string level) {
	GameState * state = new GameState();

	//get the scene parsed json
	JsonReader* jReader_ = JsonReader::getSingleton();
	auto scene = jReader_->ReadLevel(level);

	//name
	state->setStateID(scene.SceneName);

	//create the gameObjects
	for (auto & go_struct : scene.gameObjects) {

		//go cfg
		auto go = new GameObject(go_struct.go_cfg);

		//all its comps created cfg and added
		go->addComponent(ComponentFactory::ParseComponents(go, &go_struct.components_cfg));

		//push the go with its config  and built comps
		state->addGameObject(go);
	}

	return state;
}


//////////////////////////////////////////////////////////////////////////////

//pushes over the current state (no delete, no pop)
void GameStateMachine::pushState(GameState *newState) {
	states_.push(newState);
	newState->setUp();
}

void GameStateMachine::popState() {
	if (!states_.empty()) {
		delete states_.top(); //delete before popping
		states_.pop();
	}
}

//deletes and pops all the states befor pushing the new one
void GameStateMachine::changeState(GameState *newState) {
	while (!states_.empty()) {
		delete states_.top(); //delete before popping
		states_.pop();
	}

	states_.push(newState);
}

GameState * GameStateMachine::getState() {
	if (!states_.empty())
		return states_.top();
	else
		return nullptr;
}

//////////////////////////////////////////////////////////////////////////////

//quizas podrian recorrer directamente ellos las listas de entidades y nos ahorramos un step

bool GameStateMachine::handleEvents(const SDL_Event evt) {
	if (states_.empty())  return false;
	return states_.top()->handleEvents(evt);
}

void GameStateMachine::update(float time) {
	if (!states_.empty()) {
		states_.top()->update(time);
	}
}