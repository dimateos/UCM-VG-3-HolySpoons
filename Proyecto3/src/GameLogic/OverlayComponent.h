//Nap_Time_Studios
#ifndef OVERLAYCOMPONENT_H_
#define OVERLAYCOMPONENT_H_

#include "Component.h"
#include <RenderSystemInterface.h>

class OverlayComponent : public Component
{
public:
	inline OverlayComponent() : Component() {};
	inline OverlayComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~OverlayComponent() { setDown(); };

	virtual void setUp();

	void hideOverlay();
	void showOverlay();

protected:
	virtual void setDown();

	Ogre::OverlayContainer* panel = nullptr;
	Ogre::Overlay *overlay = nullptr;
};

#endif /* OVERLAYCOMPONENT_H_ */