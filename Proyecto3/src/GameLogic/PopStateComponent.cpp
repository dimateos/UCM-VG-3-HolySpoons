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
			//cambio de rendering target
			RenderSystemInterface::getSingleton()->setRenderingScene(state);
			GameStateMachine::getSingleton()->popState();

			//activar overlays del estado que viene
			GameObject* o = GameStateMachine::getSingleton()->currentState()->getGameObject("gm_go");
			MessageSystem::getSingleton()->sendMessageGameObject(&Message(ACTIVATE_UI), o);

			//pause/unpause physics
			PhysicsSystemManager::getSingleton()->pausePhysics(state == mainGameState);
			TimeSystem::StartCounter();

			return true;
		}
	}

	return false;
}

#include "GOFactory.h"
REGISTER_TYPE(PopStateComponent);
