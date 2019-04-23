#ifndef CURSORCOMPONENT_H_
#define CURSORCOMPONENT_H_

#include "Component.h"
#include <Windows.h> //just for windows lel

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
	HCURSOR c;
public:
	CursorComponent() {};
	virtual ~CursorComponent() {};
	CursorComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();

	virtual void update(GameObject* o, double time);
};

#endif //CURSORCOMPONENT_H_