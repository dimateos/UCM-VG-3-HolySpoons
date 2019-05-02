#include "CursorComponent.h"

#include <SDL_events.h>	//events
#include "Messages.h"

void CursorComponent::setUp()
{
	if (isInited()) return;
	setInited();

	string path = this->cfg_["image"];
	c = LoadCursorFromFileA(path.c_str());
}

void CursorComponent::update(GameObject * o, double time)
{
	//change the original cursor for the game's
	SetCursor(c);
}

void CursorComponent::receive(Message * msg) {
	if (msg->id_ == STATE_CHANGED) {
		//show and no capture mouse
		//SDL_ShowCursor(SDL_ENABLE); //no need
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(CursorComponent);