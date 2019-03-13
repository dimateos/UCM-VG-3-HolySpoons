#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "GameObject.h"

// every type of component will inherit from this class
class Component : public Activable
{
public:
	Component() { setUp(); }
	virtual ~Component() { setDown(); }

	// called from gameObjects themselves
	virtual void update(GameObject* ent, float time) = 0;
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt) = 0;

	virtual void setUp() = 0;   // set up in Ogre/physx
	virtual void setDown() = 0; // set down from Ogre/physx
};

#endif /* COMPONENT_H_ */
