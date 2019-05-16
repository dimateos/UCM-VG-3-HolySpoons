#include "PopStateComponent.h"
#include <SDL_events.h>
#include <KeyMapper.h>


#include "GameStateMachine.h"
#include "Messages.h"

void PopStateComponent::setUp() {
	if (isInited()) return;
	setInited();

	//key to press
	std::string k = this->cfg_["key"];
	key = KeyMapper::getKeycode(k);

	//name of the target state
	std::string s = this->cfg_["state"];
	state = s;

	//or pop simple, which doesnt need name
	popSimple = FINDnRETURN(cfg_, "popSimple", bool, false);
}

void PopStateComponent::popState() {
	if (popSimple) GameStateMachine::getSingleton()->popState();
	else GameStateMachine::getSingleton()->popToState(state);
}

bool PopStateComponent::handleEvents(GameObject * o, const SDL_Event & evt) {
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == key) {
			popState();
			return true;
		}
	}

	return false;
}

//other methods to pop like pressing a button
void PopStateComponent::receive(Message * msg) {
	if (msg->id_ == POP_STATE) {
		popState();
	}
}

#include "GOFactory.h"
REGISTER_TYPE(PopStateComponent);
