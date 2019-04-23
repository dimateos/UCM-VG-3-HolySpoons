#include "CursorComponent.h"
#include <RenderSystemInterface.h>
#include <OgreOverlayElement.h>

#include <SDL_events.h>	//events

void CursorComponent::setUp()
{
	if (isInited()) return;
	setInited();
	string path = this->cfg_["image"];
	c = LoadCursorFromFileA(path.c_str());
}

bool CursorComponent::handleEvents(GameObject * o, const SDL_Event & evt)
{
	/*if (evt.type == SDL_MOUSEMOTION) {
		SDL_GetMouseState(&x, &y);
		RenderSystemInterface::getSingleton()->setOverlayElementPosition(image, x, y);
	}*/

	SetCursor(c);
	return false;
}

#include "GOFactory.h"
REGISTER_TYPE(CursorComponent);