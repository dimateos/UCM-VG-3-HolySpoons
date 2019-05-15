#include "GameStateMachine.h"

#include <SDL_events.h>	//fowarded events

//required
#include "GOFactory.h"		//build comps
#include "GameObject.h"
#include <Transforms.h>

//reset stuff on state changed
#include "MessageSystem.h"
#include <SoundManager.h>
#include <RenderSystemManager.h>
#include <RenderSystemInterface.h>
#include <PhysicsSystemManager.h>
#include <TimeSystem.h>


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
	GOStruct* manager = nullptr;
	auto scene = JsonReader::getSingleton()->ReadLevel(level, player, manager);

	//name
	state->setStateID(scene.SceneName);

	//load ogre scene
	RenderSystemManager::getSingleton()->setupScene(scene.SceneName);

	//set player in the state and as listener, if it exists
	auto sm = SoundManager::getSingleton();
	if (player != nullptr) {
		state->setPlayer(GOFactory::ParseGO(*player));
		sm->setListenerTransform(state->getPlayer()->getTransPtr());
	}
	else {
		sm->setListenerTransform(new nap_transform(nap_vector3(0, 0, 0)));
	}

	// set the manager int the state, if it exists
	if (manager != nullptr) {
		state->setGM(GOFactory::ParseGO(*manager));
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

//reset stuff required after a state change
void GameStateMachine::preStateChanged() {
	GameState* current = currentState();
	std::string id = current->getStateID();

	//sounds
	SoundManager::getSingleton()->stopSounds();

	//set rendering scene
	RenderSystemInterface::getSingleton()->setRenderingScene(id);

	//pause/unpause physics
	PhysicsSystemManager::getSingleton()->pausePhysics(id != mainGameStateName);

	//update message targets
	MessageSystem::getSingleton()->updateTargets(current->getGameObjects());
}
void GameStateMachine::postStateChanged() {
	//send message
	MessageSystem::getSingleton()->sendMessage(&Message(STATE_IN));

	//reset time counter
	TimeSystem::StartCounter();
}

//////////////////////////////////////////////////////////////////////////////

GameState * const GameStateMachine::currentState() {
	if (!states_.empty()) return states_.top();
	else return nullptr;
}

//pushes over the current state (no delete, no pop)
void GameStateMachine::pushState(GameState *newState) {
	states_.push(newState);
	preStateChanged();
	newState->setUp();
	postStateChanged();
}

//pops until target state is reached
void GameStateMachine::popToState(std::string targetName) {
	while (!states_.empty() && targetName != states_.top()->getStateID()) {
		delete states_.top(); //delete before popping
		states_.pop();
	}

	preStateChanged();
	postStateChanged();
}

//just pops one state
void GameStateMachine::popState() {
	if (!states_.empty()) {
		delete states_.top(); //delete before popping
		states_.pop();

		preStateChanged();
		postStateChanged();
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
void GameStateMachine::clearToState(GameState *newState) {
	clearStates();
	pushState(newState);
}

bool const GameStateMachine::stackIsEmpty() const
{
	return states_.empty();
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
