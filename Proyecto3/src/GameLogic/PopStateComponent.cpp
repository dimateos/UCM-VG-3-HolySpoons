#include "PopStateComponent.h"

#include "GameStateMachine.h"
#include "Messages.h"

void PopStateComponent::setUp() {
	if (isInited()) return;
	setInited();

	//key to press
	std::string keycode = this->cfg_["key"];
	if (keycode == "esc") {
		key = SDLK_ESCAPE; //cant read special chars
	}
	else {
		key = SDL_Keycode(keycode[0]);
	}

	//name of the target state
	std::string s = this->cfg_["state"];
	state = s;
}

void PopStateComponent::popState() {
	GameStateMachine::getSingleton()->popToState(state);
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
