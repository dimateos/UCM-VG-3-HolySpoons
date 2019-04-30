#include "PopStateComponent.h"

#include "GameStateMachine.h"
#include <RenderSystemInterface.h>
#include "MessageSystem.h"
#include <PhysicsSystemManager.h>
#include <TimeSystem.h>

void PopStateComponent::setUp()
{
	if (isInited()) return;
	setInited();

	string keycode = this->getCfg()["key"];

	if (keycode == "esc") {
		key = SDLK_ESCAPE; //cant read special chars
	}
	else {
		key = SDL_Keycode(keycode[0]);
	}

	state = this->getCfg()["state"];
}

bool PopStateComponent::handleEvents(GameObject * o, const SDL_Event & evt)
{
	if (evt.type == SDL_KEYDOWN) {
		if (evt.key.keysym.sym == key) {
			popState();

			return true;
		}
	}

	return false;
}

void PopStateComponent::popState() {
	string s = state;

	while (GameStateMachine::getSingleton()->stackSize() > 0
		&& s != GameStateMachine::getSingleton()->currentState()->getStateID()) {
		GameStateMachine::getSingleton()->popState();
	}

	RenderSystemInterface::getSingleton()->setRenderingScene(s);

	//activar overlays del estado que viene
	MessageSystem::getSingleton()->updateTargets(GameStateMachine::getSingleton()->currentState()->getGameObjects());
	MessageSystem::getSingleton()->sendMessage(&Message(MessageId::STATE_CHANGED));

	//pause/unpause physics
	PhysicsSystemManager::getSingleton()->pausePhysics(state == mainGameState);
	TimeSystem::StartCounter();
}

void PopStateComponent::receive(Message * msg)
{
	if (msg->id_ == POP_STATE) {
		popState();
	}
}

#include "GOFactory.h"
REGISTER_TYPE(PopStateComponent);
