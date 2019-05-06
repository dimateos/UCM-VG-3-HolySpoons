#include "PushStateComponent.h"
#include <SDL_events.h>

#include <RenderSystemInterface.h>
#include <RenderSystemManager.h>
#include "OverlayComponent.h"
#include "GameStateMachine.h"
#include "Messages.h"

void PushStateComponent::setUp() {
	if (isInited()) return;
	setInited();

	std::string keycode = this->cfg_["key"];

	if (keycode == "esc") {
		key = SDLK_ESCAPE; //cant read special chars
	}
	else {
		key = SDL_Keycode(keycode[0]);
	}

	string  j = this->cfg_["json"];
	json = j;
}

void PushStateComponent::pushState()
{
	//cambio de rendering target
	static_cast<OverlayComponent*>(owner_->getComponent("canvas"))->hideOverlay();

	//cambio de estado
	state = GameStateMachine::getSingleton()->loadLevel(json); //CANT BE READ IT IN CONSTRUCTOR, POPSTATE DELETES IT
	GameStateMachine::getSingleton()->pushState(state);
}

bool PushStateComponent::handleEvents(GameObject * o, const SDL_Event & evt)
{
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == key) {
			pushState();
			return true;
		}
	}

	return false;
}

void PushStateComponent::receive(Message * msg)
{
	if (msg->id_ == PUSH_STATE) {
		pushState();
	}
}

#include "GOFactory.h"
REGISTER_TYPE(PushStateComponent);
