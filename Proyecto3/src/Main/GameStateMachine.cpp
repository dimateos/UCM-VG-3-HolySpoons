#include "GameStateMachine.h"

GameStateMachine::GameStateMachine() {}

GameStateMachine::~GameStateMachine() {
	while (!states_.empty()) {
		//delete states_.top(); //delete its states
		states_.pop();
	}
}

void GameStateMachine::receive(Message* msg) {
	switch (msg->id_) {
	default:
		break;
		//msg recieved
	}

	//sounds
	//game_->sendToSM(msg);
}

//////////////////////////////////////////////////////////////////////////////

//pushes over the current state (no delete, no pop)
void GameStateMachine::pushState(GameState *newState) {
	states_.push(newState);
}

void GameStateMachine::popState() {
	if (!states_.empty()) {
		//	delete states_.top(); //delete before popping
		states_.pop();
	}
}

//deletes and pops all the states befor pushing the new one
void GameStateMachine::changeState(GameState *newState) {
	while (!states_.empty()) {
		//delete states_.top(); //delete before popping
		states_.pop();
	}

	states_.push(newState);
}

//////////////////////////////////////////////////////////////////////////////

//quizas podrian recorrer directamente ellos las listas de entidades y nos ahorramos un step

void GameStateMachine::handleEvents(float time, const Event evt) {
	if (!states_.empty()) {
		states_.top()->handleEvents(time, evt);
	}
}

void GameStateMachine::update(float time) {
	if (!states_.empty()) {
		states_.top()->update(time);
	}
}