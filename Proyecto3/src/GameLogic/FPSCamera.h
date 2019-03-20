//Nap_Time_Studios
#pragma once

#include <OgreSceneNode.h>
#include "OgreText.h"

#include "Component.h"
class FPSCamera : public Component
{
public:
	FPSCamera() : Component() { setUp(); };
	virtual ~FPSCamera() { setDown(); };

	//smooth movement -> updates all the time
	virtual void update(GameObject* ent, float time);
	//mouse move + rightClick and screen resize
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt);

protected:
	virtual void setUp();
	virtual void setDown();

	//center it
	void updateCrosshair();
	OgreText* x;

	void toggleZoom();
	float zoomed = 2500.0; //amount of zoom

	Ogre::SceneNode* camNode_;
	float rotXspeed_ = 10.5, rotYspeed_ = 10.5; //sens
	float rotX_, rotY_; //updaters
};