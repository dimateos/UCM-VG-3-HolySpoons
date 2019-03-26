#pragma once
#include "Component.h"
#include <RenderSystemInterface.h>

class OverlayComponent :
	public Component
{
private:
	Ogre::OverlayContainer* panel = nullptr;
	Ogre::Overlay *overlay = nullptr;

protected:
	virtual void setUp(nap_json const & cfg);
	virtual void setDown();

public:
	OverlayComponent() {};
	OverlayComponent(nap_json cfg) : Component(cfg) { this->setUp(cfg); };
	virtual ~OverlayComponent() { setDown(); };
	
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt) { return false; };
	virtual void update(GameObject* ent, float time) {};
	virtual void late_update(GameObject* ent, float time) {};

	void hideOverlay();
	void showOverlay();
};
