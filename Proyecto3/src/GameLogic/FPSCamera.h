//Nap_Time_Studios
#pragma once

#include "RenderSystemInterface.h"

#include "Component.h"
class FPSCamera : public Component
{
public:
	inline FPSCamera(nap_json const & cfg) : Component(cfg) { this->setUp(cfg); };
	inline virtual ~FPSCamera() { setDown(); };

	//mouse move + rightClick and screen resize
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt);
	//smooth movement -> updates all the time
	virtual void update(GameObject* ent, float time);

	virtual void loadParameters(std::vector<string>parameters);

protected:
	virtual void setUp(nap_json const & cfg);
	virtual void setDown();

	void toggleZoom();
	float zoomed = 2500.0; //amount of zoom

	Ogre::SceneNode* camNode_;
	float rotXspeed_ = 10.5, rotYspeed_ = 10.5; //sens
	float rotX_, rotY_; //updaters
};