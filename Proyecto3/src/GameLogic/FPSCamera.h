//Nap_Time_Studios
#pragma once

#include "RenderSystemInterface.h"

#include "Component.h"
class FPSCamera : public Component
{
public:
	inline FPSCamera(nap_json const & cfg) : Component(cfg) {};
	inline virtual ~FPSCamera() {};
	virtual void setUp();

	//mouse move + rightClick and screen resize
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt);
	//smooth movement -> updates all the time
	virtual void update(GameObject* ent, double time);

protected:
	void toggleZoom();

	//logic
	Ogre::SceneNode* camNode_;
	float rotX_, rotY_; //updaters
	bool zoom = false;

	//cfg (json)
	float rotXspeed_, rotYspeed_, zoomed_;
};