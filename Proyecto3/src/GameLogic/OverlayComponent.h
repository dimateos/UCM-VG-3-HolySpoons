//Nap_Time_Studios
#ifndef OVERLAYCOMPONENT_H_
#define OVERLAYCOMPONENT_H_

#include "Component.h"
#include <RenderSystemInterface.h>


namespace Ogre {
	class OverlayElement;
}

class OverlayComponent : public Component
{
protected:
	virtual void setDown();

	Ogre::OverlayContainer* panel = nullptr;
	Ogre::Overlay *overlay = nullptr;

public:
	inline OverlayComponent() {};
	inline OverlayComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~OverlayComponent() { setDown(); };

	virtual void setUp();

	void hideOverlay();
	void showOverlay();

	Ogre::OverlayElement* getOverlayElementByName(std::string name);
};

#endif /* OVERLAYCOMPONENT_H_ */