#include "GameStateMachine.h"
#include "MessageSystem.h"

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

#include "GOFactory.h" //build comps
#include "JsonReader.h" //reading levels

GameState * GameStateMachine::loadLevel(std::string level) {
	GameState * state = new GameState();

	//get the scene parsed json
	JsonReader* jReader_ = JsonReader::getSingleton();
	auto scene = jReader_->ReadLevel(level);

	//name
	state->setStateID(scene.SceneName);
	
	//player
	GOStruct player = jReader_->ReadPlayer(level);
	state->setPlayer(GOFactory::ParseGO(player));

	//create the gameObjects
	for (auto & go_struct : scene.gameObjects) {

		//go cfg and components
		auto go = GOFactory::ParseGO(go_struct);

		//push the go with its config  and built comps
		state->addGameObject(go);
	}

	return state;
}

//////////////////////////////////////////////////////////////////////////////

GameState * const GameStateMachine::currentState() {
	if (!states_.empty())
		return states_.top();
	else
		return nullptr;
}

//pushes over the current state (no delete, no pop)
void GameStateMachine::pushState(GameState *newState) {
	states_.push(newState);
	newState->setUp();
	//this needs to be done everytime we change state
	MessageSystem::getSingleton()->updateTargets(newState->getGameObjects());
}

void GameStateMachine::popState() {
	if (!states_.empty()) {
		delete states_.top(); //delete before popping
		states_.pop();
	}
}

//deletes and pops all the states befor pushing the new one
void GameStateMachine::clearStates() {
	while (!states_.empty()) {
		delete states_.top(); //delete before popping
		states_.pop();
	}
}

//deletes and pops all the states befor pushing the new one
void GameStateMachine::changeState(GameState *newState) {
	clearStates();
	pushState(newState);
}

//////////////////////////////////////////////////////////////////////////////

//quizas podrian recorrer directamente ellos las listas de entidades y nos ahorramos un step

bool GameStateMachine::handleEvents(const SDL_Event evt) {
	if (states_.empty())  return false;
	return states_.top()->handleEvents(evt);
}

void GameStateMachine::update(double time) {
	if (!states_.empty()) {
		states_.top()->update(time);
	}
}