#pragma once
#include "Component.h"
#include <RenderSystemInterface.h>

class OverlayComponent : public Component
{
public:
	inline OverlayComponent() : Component() {};
	inline OverlayComponent(nap_json cfg) : Component(cfg) {};
	virtual ~OverlayComponent() { setDown(); };

	virtual void setUp();

	void hideOverlay();
	void showOverlay();

protected:
	virtual void setDown();

	Ogre::OverlayContainer* panel = nullptr;
	Ogre::Overlay *overlay = nullptr;
};
