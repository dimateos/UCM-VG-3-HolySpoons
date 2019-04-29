#include "GameStateMachine.h"

#include <SDL_events.h>	//fowarded events

//required
#include "MessageSystem.h"	//update the MessageSystem targets
#include "GOFactory.h"		//build comps
#include "GameObject.h"
#include <Transforms.h>
#include <SoundManager.h>	//tmp play music

GameStateMachine* GameStateMachine::instance_ = nullptr;
GameStateMachine::GameStateMachine() {}

GameStateMachine::~GameStateMachine() {
	while (!states_.empty()) {
		delete states_.top(); //delete its states
		states_.pop();
	}
}

GameStateMachine * GameStateMachine::getSingleton()
{
	if (instance_ == nullptr) {
		instance_ = new GameStateMachine();
	}

	return instance_;
}

void GameStateMachine::shutdownSingleton()
{
	delete instance_;
}

///////////////////////////////////////////////////////////////////////////////

GameState * GameStateMachine::loadLevel(std::string level) {
	GameState * state = new GameState();

	//get the scene parsed json
	GOStruct* player = nullptr;
	auto scene = JsonReader::getSingleton()->ReadLevel(level, player);

	//name
	state->setStateID(scene.SceneName);

	//set player as listener if exists
	auto sm = SoundManager::getSingleton();
	if (player != nullptr) {
		state->setPlayer(GOFactory::ParseGO(*player));
		sm->setListenerTransform(state->getPlayer()->getTransPtr());
	}
	else {
		sm->setListenerTransform(new nap_transform(nap_vector3(0, 0, 0)));
	}

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

		//this needs to be done everytime we change state
		MessageSystem::getSingleton()->updateTargets(currentState()->getGameObjects());
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