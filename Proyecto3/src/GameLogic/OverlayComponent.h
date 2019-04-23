//Nap_Time_Studios
#ifndef OVERLAYCOMPONENT_H_
#define OVERLAYCOMPONENT_H_

#include "Component.h"

namespace Ogre {
	class OverlayElement;
	class OverlayContainer;
	class Overlay;
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
	virtual void lateSetUp();

	void hideOverlay();
	void showOverlay();

	Ogre::OverlayElement* getOverlayElementByName(std::string name);
	void hidePanelByName(std::string name);
	void showPanelByName(std::string name);

	virtual void receive(Message* msg);
};

#endif /* OVERLAYCOMPONENT_H_ */