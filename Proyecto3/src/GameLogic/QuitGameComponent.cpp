#include "QuitGameComponent.h"
#include "Messages.h"
#include "GameStateMachine.h"
#include <SDL.h>

void QuitGameComponent::clearStates()
{
	GameStateMachine::getSingleton()->clearStates();
}

void QuitGameComponent::setUp() {
	if (isInited()) return;
	setInited();	
}

bool QuitGameComponent::handleEvents(GameObject * o, const SDL_Event & evt)
{
	bool handled = false;

	if (evt.type == SDL_KEYDOWN) {
		switch (evt.key.keysym.sym) {
			case SDLK_ESCAPE:
				handled = true;
				clearStates();
				break;
			}
	}

	return handled;
}

void QuitGameComponent::receive(Message * msg)
{
	if (msg->id_ == QUIT_GAME) {
		clearStates();
	}
}

#include "GOFactory.h"
REGISTER_TYPE(QuitGameComponent);
