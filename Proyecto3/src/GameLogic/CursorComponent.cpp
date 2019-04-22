#include "CursorComponent.h"
#include <RenderSystemInterface.h>
#include <OgreCamera.h>
#include <OgreViewport.h>

#include <SDL_events.h>	//events

void CursorComponent::setUp()
{
	if (isInited()) return;
	setInited();

	image = RenderSystemInterface::getSingleton()->getOverlayElement("Cursor");
}

bool CursorComponent::handleEvents(GameObject * o, const SDL_Event & evt)
{
	if (evt.type == SDL_MOUSEMOTION) {
		SDL_GetMouseState(&x, &y);
		RenderSystemInterface::getSingleton()->setOverlayElementPosition(image, x, y);
	}

	return false;
}

#include "GOFactory.h"
REGISTER_TYPE(CursorComponent);