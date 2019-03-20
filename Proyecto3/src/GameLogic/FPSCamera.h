//Nap_Time_Studios
#pragma once

#include <OgreSceneNode.h>

#include "Component.h"
class FPSCamera : public Component
{
public:
	FPSCamera() : Component() { setUp(); };
	virtual ~FPSCamera() { setDown(); };

	virtual void update(GameObject* ent, float time);
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt);

protected:
	virtual void setUp();
	virtual void setDown();

	void toggleZoom();
	float zoomed = -2500.0;

	float rotXspeed_ = 1.5, rotYspeed_ = 1.5;
	float rotX_, rotY_;

	Ogre::Node* camNode_;
	Ogre::Camera* cam_;
};