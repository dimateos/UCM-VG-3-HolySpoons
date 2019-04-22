#ifndef CURSORCOMPONENT_H_
#define CURSORCOMPONENT_H_

#include "Component.h"

namespace Ogre {
	class Camera;
	class OverlayElement;
}

class RenderSystemInterface;

class CursorComponent : public Component
{
private:
	int x, y;
	Ogre::OverlayElement* image = nullptr;
	RenderSystemInterface* rsi = nullptr;

public:
	CursorComponent() {};
	virtual ~CursorComponent() {};
	CursorComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();

	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
};

#endif //CURSORCOMPONENT_H_