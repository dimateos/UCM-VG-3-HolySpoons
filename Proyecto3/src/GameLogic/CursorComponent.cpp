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

void CursorComponent::update(GameObject * o, double time)
{
	SetCursor(c);
}

#include "GOFactory.h"
REGISTER_TYPE(CursorComponent);